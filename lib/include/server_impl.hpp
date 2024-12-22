#pragma once

#include <memory>
#include <string_view>
#include <thread>
#include <deque>
//#include <unordered_map>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>


namespace sb::sleeping_dog {

class router;
class listener;
class http_session;

class server_impl
  : public std::enable_shared_from_this<server_impl>
{
  struct private_token { };
public:
  server_impl(private_token key, size_t thread_count);
  virtual ~server_impl();

  static std::shared_ptr<server_impl> create(size_t thread_count);


  void listen(std::string_view addr, unsigned port);
  void listen(unsigned port);

  void take(std::unique_ptr<router>&& router);

  /// @throws smth if router is nullptr
  sleeping_dog::router* router();

  /// blocking call
  void run();

  /// non-blocking call
  void start();
  void join();
  bool try_join();

  void cancel();


  void session_add(std::shared_ptr<http_session> session_ptr);
  void session_del(std::shared_ptr<http_session> session_ptr);


private:

  void initialize();

private:

  size_t thread_count_;
  boost::asio::io_context ioc_;
  boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;
  std::vector<std::thread> threads_;

  std::string address_str_ = "0.0.0.0";
  unsigned port_ = 8080;


  std::unique_ptr<sleeping_dog::router> router_;


  std::unique_ptr<boost::asio::ssl::context> ctx_;
  std::shared_ptr<listener> listener_;
  //std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor_;

  std::deque<std::shared_ptr<http_session>> sessions_;

};


} // namespace sb::sleeping_dog
