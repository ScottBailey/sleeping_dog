#pragma once

#include <string>
#include <memory>
#include <functional>

#include <sb/sleeping_dog/types.hpp>


namespace sb::sleeping_dog {



class router {
public:

  router();
  ~router();




  using auth_type = std::string;

  using callback_type = std::function<response_type(const request_type&)>;



  void prefix(path_type path);
  void auth(auth_type);
  void add(verb_type verb, path_type path, callback_type cb);
  void drop(verb_type verb, path_type path);

  // Handling a request requires a response.
  response_type handle(const request_type& req);

private:
  struct router_data;   // forward declaration
  std::unique_ptr<router_data> data_;
};


} // namespace sb::sleeping_dog
