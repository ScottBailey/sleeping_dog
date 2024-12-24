#include <http_session.hpp>
#include <server_impl.hpp>


namespace {

void fail(boost::beast::error_code bec, std::string_view what) {
    // ssl::error::stream_truncated, also known as an SSL "short read",
    // indicates the peer closed the connection without performing the
    // required closing handshake (for example, Google does this to
    // improve performance). Generally this can be a security issue,
    // but if your communication protocol is self-terminated (as
    // it is with both HTTP and WebSocket) then you may simply
    // ignore the lack of close_notify.
    //
    // https://github.com/boostorg/beast/issues/38
    //
    // https://security.stackexchange.com/questions/91435/how-to-handle-a-malicious-ssl-tls-shutdown
    //
    // When a short read would cut off the end of an HTTP message,
    // Beast returns the error beast::http::error::partial_message.
    // Therefore, if we see a short read here, it has occurred
    // after the message has been completed, so it is safe to ignore it.

  if(bec == boost::asio::ssl::error::stream_truncated)
      return;

  spdlog::error("[Fail] [what: '{}'] [ec: '{}']", what, bec.message());
}

} // namespace



namespace sb::sleeping_dog {

//---

http_session::http_session(private_token /*key*/, std::weak_ptr<server_impl> server_ptr, boost::asio::ip::tcp::socket&& socket, boost::asio::ssl::context& ctx)
  : server_{server_ptr}
  , stream_(std::move(socket), ctx)
{
  {
    // create the base name
    std::ostringstream oss;
    oss << "http_session: " << stream_.lowest_layer().remote_endpoint().address();
    auto name = oss.str();
    // see if we need to add instance
    logger_ = spdlog::get(name);
    if (logger_) {
      std::string temp_name;
      temp_name = name;
      for (size_t i = 1; logger_; ++i) {
        spdlog::debug("logger: '{}' exists", temp_name);
        temp_name = fmt::format("{} {}", name, i);
        logger_ = spdlog::get(temp_name);
      }
      name = temp_name;
    }
    // create the logger
    log_name_ = name;
    logger_ = spdlog::stdout_color_mt(log_name_);
  }

  // might be noce to add what connected here.
  logger_->trace("[constructor]");
}


http_session::~http_session() {
  logger_->trace("[destructor]");
  spdlog::drop(log_name_);
}


//---


std::shared_ptr<http_session> http_session::create(std::weak_ptr<server_impl> server_ptr, boost::asio::ip::tcp::socket&& socket, boost::asio::ssl::context& ctx) {
  auto rv = std::make_shared<http_session>(private_token{}, server_ptr, std::move(socket), ctx);
  rv->run();
  return rv;
}



void http_session::do_close() {
  logger_->trace("[do_close]");

  auto server_ptr = server_.lock();
  if (!server_ptr) {
    return;
  }

  server_ptr->session_del(shared_from_this());
}


void http_session::do_read() {
  logger_->trace("[do_read]");

  // Set the timeout.
  boost::beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

  // Make the request empty before reading,
  // otherwise the operation behavior is undefined.
  request_ = {};

  // Read a request
  boost::beast::http::async_read(stream_, buffer_, request_,
      [self = shared_from_this()](boost::beast::error_code bec, std::size_t bytes_transferred) {
        self->on_read(bec,bytes_transferred);
      });
}


void http_session::on_handshake(boost::beast::error_code bec) {
  logger_->trace("[on_handshake]");

  // ~/.cpm/boost/1359e136761ab2d10afa1c4e21086c8d824735cd/libs/beast/example/http/server/async-ssl/http_server_async_ssl.cpp
  if(bec) {
    logger_->error("[on_handshake] [error: '{}']", bec.what());
    return fail(bec, "handshake");
  }

  do_read();
}


void http_session::on_read(boost::beast::error_code bec, std::size_t /*bytes_transferred*/) {
  logger_->trace("[on_read]");

  // This means they closed the connection
  if(bec == boost::beast::http::error::end_of_stream)
    return do_close();

  if(bec) {
    logger_->error("[on_read] [error: '{}']", bec.what());
    return fail(bec, "read");
  }

  auto server = server_.lock();
  if (!server) {
    logger_->error("[on_read] [error: NO SERVER]");
    return fail(bec, "no server");
  }

  // Create and send the response
  auto response = server->handle_request(std::move(request_));
  send_response(std::move(response));
}


void http_session::on_run() {
  logger_->trace("[on_run]");

  auto server_ptr = server_.lock();
  if (!server_ptr) {
    return;
  }

  // Set the timeout.
  boost::beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

  // Perform the SSL handshake
  stream_.async_handshake(boost::asio::ssl::stream_base::server,
      [self = shared_from_this()](boost::beast::error_code bec) {
        self->on_handshake(bec);
      });
}


void http_session::on_write(boost::beast::error_code bec, size_t /*bytes_transferred*/) {
  logger_->trace("[on_write]");

  if(bec == boost::beast::http::error::end_of_stream)
    return do_close();

  if(bec) {
    logger_->error("[on_write] [error: '{}']", bec.what());
    return fail(bec, "write");
  }

  do_read();
}


void http_session::run() {
  logger_->trace("[run]");

  // move the run call into the io_context
  auto server_ptr = server_.lock();
  if (!server_ptr) {
    logger_->error("[run] [error: NO SERVER]");
    return;
  }
  auto self_ptr = weak_from_this().lock();
  if (!self_ptr) {
    spdlog::critical("[http_session::run() called without a self!]");
    return;
  }
  server_ptr->session_add(shared_from_this());
  boost::asio::dispatch(stream_.get_executor(),
      [self = shared_from_this()]() {
        self->on_run();
      });
}


void http_session::send_response(response_type&& r) {
  logger_->trace("[send_response]");

  response_ = std::move(r);

  boost::beast::http::async_write(stream_, response_,
      [self = shared_from_this()](boost::beast::error_code bec, size_t bytes_transferred) {
        self->on_write(bec, bytes_transferred);
      });

}




} // namespace sb::sleeping_dog
