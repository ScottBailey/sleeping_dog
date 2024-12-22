#include <http_session.hpp>
#include <server_impl.hpp>


#include <iostream> // debugging, remove!!!!

namespace {

void fail(boost::beast::error_code ec, std::string_view what) {
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

  if(ec == boost::asio::ssl::error::stream_truncated)
      return;

  std::cerr << what << ": " << ec.message() << "\n";
}

} // namespace



namespace sb::sleeping_dog {

//---

http_session::http_session(private_token /*key*/, std::weak_ptr<server_impl> server_ptr, boost::asio::ip::tcp::socket&& socket, boost::asio::ssl::context& ctx)
  : server_{server_ptr}
  , stream_(std::move(socket), ctx)
{
  static_assert(queue_limit_ > 0, "queue limit must be positive");
  std::cout << "http_session::http_session()" << std::endl;
}


http_session::~http_session() = default;


//---


std::shared_ptr<http_session> http_session::create(std::weak_ptr<server_impl> server_ptr, boost::asio::ip::tcp::socket&& socket, boost::asio::ssl::context& ctx) {
  std::cout << "http_session::create()" << std::endl;
  auto rv = std::make_shared<http_session>(private_token{}, server_ptr, std::move(socket), ctx);
  rv->run();
  return rv;
}



void http_session::do_close() {

  auto server_ptr = server_.lock();
  if (!server_ptr) {
    return;
  }

  server_ptr->session_del(shared_from_this());
}


void http_session::do_read() {
  std::cout << "http_session::do_read()" << std::endl;
  // Set the timeout.
  boost::beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

  // Make the request empty before reading,
  // otherwise the operation behavior is undefined.
  req_ = {};

  // Read a request
  boost::beast::http::async_read(stream_, buffer_, req_,
      [self = shared_from_this()](boost::beast::error_code ec, std::size_t bytes_transferred) {
        self->on_read(ec,bytes_transferred);
      });
}


response_type http_session::handle_request(request_type&& r) {

  // Find the path
  std::cout << r.target() << std::endl;


  // Return a failure response here...
  return response_type{}; // ***fix***
}


void http_session::on_handshake(boost::beast::error_code ec) {
  std::cout << "http_session::on_handshake()" << std::endl;

  // ~/.cpm/boost/1359e136761ab2d10afa1c4e21086c8d824735cd/libs/beast/example/http/server/async-ssl/http_server_async_ssl.cpp
  if(ec)
    return fail(ec, "handshake");

  do_read();
}


void http_session::on_read(boost::beast::error_code bec, std::size_t /*bytes_transferred*/) {
  std::cout << "http_session::on_read()" << std::endl;
  // This means they closed the connection
  if(bec == boost::beast::http::error::end_of_stream)
    return do_close();

  if(bec)
    return fail(bec, "read");

  // Send the response

  // this is where we call the router.
  auto response = handle_request(std::move(req_));
  send_response(std::move(response));
}


void http_session::on_run() {
  std::cout << "http_session::on_run()" << std::endl;

  auto server_ptr = server_.lock();
  if (!server_ptr) {
    return;
  }

  // Set the timeout.
  boost::beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

  // Perform the SSL handshake
  stream_.async_handshake(boost::asio::ssl::stream_base::server,
      [self = shared_from_this()](boost::beast::error_code ec) {
        self->on_handshake(ec);
      });
}


void http_session::on_write(boost::beast::error_code bec, size_t /*bytes_transferred*/) {
  std::cout << "http_session::on_write()" << std::endl;

  if(bec == boost::beast::http::error::end_of_stream)
    return do_close();

  if(bec)
    return fail(bec, "read");

  do_read();
}


void http_session::run() {
  std::cout << "http_session::run()" << std::endl;
  // move the run call into the io_context
  auto server_ptr = server_.lock();
  if (!server_ptr) {
    return;
  }
  auto self_ptr = weak_from_this().lock();
  if (!self_ptr) {
    std::cout << "http_session::run() bad self" << std::endl;
    return;
  }
  server_ptr->session_add(shared_from_this());
  boost::asio::dispatch(stream_.get_executor(),
      [self = shared_from_this()]() {
        self->on_run();
      });
}


void http_session::send_response(response_type&& r) {
  std::cout << "http_session::send_response()" << std::endl;

  boost::beast::http::async_write(stream_, std::move(r),
      [self = shared_from_this()](boost::beast::error_code bec, size_t bytes_transferred) {
        self->on_write(bec, bytes_transferred);
      });

}




} // namespace sb::sleeping_dog
