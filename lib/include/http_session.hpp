#pragma once

#include <memory>
#include <queue>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>


// from:
//   boost/libs/beast/example/advanced/server/advanced_server.cpp
//   boost/libs/beast/example/http/server/async-ssl/http_server_async_ssl.cpp


namespace sb::sleeping_dog {

class http_session
  : public std::enable_shared_from_this<http_session>
{
  struct private_token {};
public:
  http_session(private_token key, boost::asio::ip::tcp::socket&& socket, boost::asio::ssl::context& ctx, const std::shared_ptr<const std::string>& doc_root);
  virtual ~http_session();

  std::shared_ptr<http_session> create(boost::asio::ip::tcp::socket&& socket, boost::asio::ssl::context& ctx, const std::shared_ptr<const std::string>& doc_root);

  void run();

private:

  void do_read();
  void on_read(boost::beast::error_code ec, std::size_t bytes_transferred);
  void queue_write(boost::beast::http::message_generator response);
  void do_write();
  void on_write(bool keep_alive, boost::beast::error_code ec, std::size_t bytes_transferred);
  void do_close();

private:
  boost::beast::ssl_stream<boost::beast::tcp_stream> stream_;
  boost::beast::flat_buffer buffer_;
  std::shared_ptr<const std::string> doc_root_;

  static constexpr std::size_t queue_limit_ = 8; // max responses
  std::queue<boost::beast::http::message_generator> response_queue_;

};

} // namespace sb::sleeping_dog
