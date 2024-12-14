#pragma once

#include <string>
#include <memory>

namespace sb::sleeping_dog {

// forward declarations
class router_impl;

class router {
public:

  router();
  ~router();

  using path_type = std::string;
  using request_type = std::string;
  using auth_type = std::string;
  using callback_type = std::string;
  using payload_type = std::string;


  void prefix(path_type);
  void auth(auth_type);
  void add(request_type, path_type, callback_type);
  void drop(request_type, path_type);


  void handle(request_type, path_type, payload_type);

private:
  std::unique_ptr<router_impl> impl_;
};


} // namespace sb::sleeping_dog
