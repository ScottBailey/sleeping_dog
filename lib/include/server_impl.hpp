#pragma once

#include <memory>
#include <string_view>
//#include <unordered_map>

namespace sb::sleeping_dog {

class router;


class server_impl
  : public std::enable_shared_from_this<server_impl>
{
  struct private_token { };
public:
  server_impl(private_token key);
  virtual ~server_impl();

  static std::shared_ptr<server_impl> create();


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

private:
  std::unique_ptr<sleeping_dog::router> router_;
};


} // namespace sb::sleeping_dog
