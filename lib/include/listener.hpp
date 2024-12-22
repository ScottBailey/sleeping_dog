#pragma once

#include <memory>
#include <string>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>


namespace sb::sleeping_dog {

class server_impl;


class listener
  : public std::enable_shared_from_this<listener>
{
  struct private_type {};
public:
  listener(private_type key, std::shared_ptr<server_impl> server_ptr,
      boost::asio::io_context& ioc, boost::asio::ssl::context& ctx, boost::asio::ip::tcp::endpoint endpoint);

  static std::shared_ptr<listener> create(std::shared_ptr<server_impl> server_ptr,
      boost::asio::io_context& ioc, boost::asio::ssl::context& ctx, boost::asio::ip::tcp::endpoint endpoint);


  void run();

private:
  void do_accept();
  void on_accept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket);


private:
  std::weak_ptr<server_impl> server_;
  boost::asio::io_context& ioc_;
  boost::asio::ssl::context& ctx_;
  boost::asio::ip::tcp::acceptor acceptor_;
};


} // namespace sb::sleeping_dog
