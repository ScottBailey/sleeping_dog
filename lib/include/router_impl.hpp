#pragma once

#include <string>
#include <memory>

namespace sb::sleeping_dog {


class router_impl {
public:

  router_impl();
  ~router_impl();

  using path_type = std::string;
  using request_type = std::string;
  using auth_type = std::string;
  using callback_type = std::string;


  void prefix(path_type);
  void auth(auth_type);
  void add(request_type, path_type, callback_type);
  void drop(request_type, path_type);
};


} // namespace sb::sleeping_dog
