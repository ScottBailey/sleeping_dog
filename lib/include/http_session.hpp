#pragma once

#include <memory>
#include <queue>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <sb/sleeping_dog/types.hpp>


// from:
//   boost/libs/beast/example/advanced/server/advanced_server.cpp
//   boost/libs/beast/example/http/server/async-ssl/http_server_async_ssl.cpp
//
// seems like the two source examples are different and we need to sort that out....


namespace sb::sleeping_dog {

class server_impl;


// Notes:
//
//  We need a way to switch to websockets.
//


class http_session
  : public std::enable_shared_from_this<http_session>
{
  struct private_token {};
public:
  http_session(private_token key, std::weak_ptr<server_impl> server_ptr, boost::asio::ip::tcp::socket&& socket, boost::asio::ssl::context& ctx);
  virtual ~http_session();

  static std::shared_ptr<http_session> create(std::weak_ptr<server_impl> server_ptr, boost::asio::ip::tcp::socket&& socket, boost::asio::ssl::context& ctx);


private:

  void run(); // move operations into the io context. Called from create().
  void on_run();
  void on_handshake(boost::beast::error_code ec);

  void do_read();
  void on_read(boost::beast::error_code ec, std::size_t bytes_transferred);
  void do_write();
  void on_write(boost::beast::error_code ec, std::size_t bytes_transferred);
  void do_close();

  void send_response(response_type&& r);

private:
  decltype(spdlog::stdout_color_mt("console")) logger_;
  std::string log_name_;

  std::weak_ptr<server_impl> server_;
  boost::beast::ssl_stream<boost::beast::tcp_stream> stream_;  // this may be deprecated?
  boost::beast::flat_buffer buffer_;

  request_type request_;
  response_type response_;
};

} // namespace sb::sleeping_dog
