#pragma once

#include <string_view>
#include <memory>


namespace sb::sleeping_dog {

// forward declarations
class router;
class server_impl;


class server {
public:
  server();
  ~server();


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
  std::shared_ptr<server_impl> impl_;
};


} // namespace sb::sleeping_dog
