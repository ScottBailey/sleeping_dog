#include <listener.hpp>
#include <string_view>

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


listener::listener(boost::asio::io_context& ioc, boost::asio::ssl::context& ctx, boost::asio::ip::tcp::endpoint endpoint, std::shared_ptr<std::string const> const& doc_root)
  : ioc_(ioc)
  , ctx_(ctx)
  , acceptor_(ioc)
  , doc_root_(doc_root)
{
  boost::beast::error_code ec;

  // Open the acceptor
  acceptor_.open(endpoint.protocol(), ec);
  if(ec) {
    fail(ec, "open");
    return;
  }

  // Allow address reuse
  acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
  if(ec) {
    fail(ec, "set_option");
    return;
  }

  // Bind to the server address
  acceptor_.bind(endpoint, ec);
  if(ec) {
    fail(ec, "bind");
    return;
  }

  // Start listening for connections
  acceptor_.listen(boost::asio::socket_base::max_listen_connections, ec);
  if(ec) {
    fail(ec, "listen");
    return;
  }

}



//---


void listener::do_accept() {
}


void listener::on_accept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket) {
}


void listener::run() {
}


} // namespace sb::sleeping_dog
