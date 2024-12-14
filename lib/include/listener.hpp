#pragma once

#include <memory>
#include <string>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>


namespace sb::sleeping_dog {

class listener
  : public std::enable_shared_from_this<listener>
{
public:
  listener(boost::asio::io_context& ioc, boost::asio::ssl::context& ctx, boost::asio::ip::tcp::endpoint endpoint, std::shared_ptr<std::string const> const& doc_root);

  void run();

private:
  void do_accept();
  void on_accept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket);


private:
  boost::asio::io_context& ioc_;
  boost::asio::ssl::context& ctx_;
  boost::asio::ip::tcp::acceptor acceptor_;
  std::shared_ptr<std::string const> doc_root_;
};


} // namespace sb::sleeping_dog
