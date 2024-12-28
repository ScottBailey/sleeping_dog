#pragma once

#include <memory>
#include <functional>

#include <sb/sleeping_dog/types.hpp>


namespace sb::sleeping_dog {


class router {
public:

  router();
  ~router();

  using callback_type = std::function<response_type(const request_type&)>;


  path_type prefix() const;
  void prefix(path_type path);

  void auth(auth::callback_type);

  void add(verb_type verb, path_type path, callback_type cb);
  void drop(verb_type verb, path_type path);

  // Handling a request requires a response.
  response_type handle(const request_type& req);


private:
  path_type prefix_;

  auth::callback_type auth_;

  struct router_data;   // forward declaration
  std::unique_ptr<router_data> route_;
};


} // namespace sb::sleeping_dog
