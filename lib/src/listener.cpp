#include <listener.hpp>
#include <server_impl.hpp>
#include <http_session.hpp>

#include <boost/asio/strand.hpp>

#include <string_view>



#include <iostream> // debugging, remove!!!!

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

  std::cerr << what << ": " << bec.message() << "\n";
}

} // namespace


namespace sb::sleeping_dog {

listener::listener(private_type /*key*/, std::shared_ptr<server_impl> server_ptr,
    boost::asio::io_context& ioc, boost::asio::ssl::context& ctx, boost::asio::ip::tcp::endpoint /*endpoint*/)
  : server_(server_ptr)
  , ioc_(ioc)
  , ctx_(ctx)
  , acceptor_(ioc)
{
}



//---

std::shared_ptr<listener> listener::create(std::shared_ptr<server_impl> server_ptr, boost::asio::io_context& ioc, boost::asio::ssl::context& ctx, boost::asio::ip::tcp::endpoint endpoint) {

  auto p = std::make_shared<listener>(private_type{}, server_ptr, ioc, ctx, endpoint);


  boost::beast::error_code bec;

  // Open the acceptor
  p->acceptor_.open(endpoint.protocol(), bec);
  if(bec) {
    fail(bec, "open");
    return {};
  }

  // Allow address reuse
  p->acceptor_.set_option(boost::asio::socket_base::reuse_address(true), bec);
  if(bec) {
    fail(bec, "set_option");
    return {};
  }

  // Bind to the server address
  p->acceptor_.bind(endpoint, bec);
  if(bec) {
    fail(bec, "bind");
    return {};
  }

  // Start listening for connections
  p->acceptor_.listen(boost::asio::socket_base::max_listen_connections, bec);
  if(bec) {
    fail(bec, "listen");
    return {};
  }

  return p;
}


void listener::do_accept() {

  acceptor_.async_accept(boost::asio::make_strand(ioc_),
      [self = shared_from_this()](boost::beast::error_code bec, boost::asio::ip::tcp::socket socket) {
        self->on_accept(bec, std::move(socket));
      });
}


void listener::on_accept(boost::beast::error_code bec, boost::asio::ip::tcp::socket socket) {
  if(bec) {
    fail(bec, "accept");
    return; // To avoid infinite loop
  }
  std::cout << "listener::on_accept()" << std::endl;
  http_session::create(server_, std::move(socket), ctx_);
  do_accept();
}


void listener::run() {
  do_accept();
}


} // namespace sb::sleeping_dog
