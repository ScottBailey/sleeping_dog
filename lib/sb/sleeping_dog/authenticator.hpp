#pragma once

#include <memory>
#include <functional>

#include <sb/sleeping_dog/types.hpp>


namespace sb::sleeping_dog {



class authenticator {
public:

  authenticator();
  ~authenticator();

  using callback_return_type = std::pair<bool,response_type>;
  using callback_type = std::function<callback_return_type(const request_type&)>;

  // Path can be empty?

  void add(verb_type verb, path_type path, callback_type cb);
  void drop(verb_type verb, path_type path);

  // Handling a request requires a response.
  response_type handle(const request_type& req);

private:
  struct auth_data;   // forward declaration
  std::unique_ptr<auth_data> data_;
};


} // namespace sb::sleeping_dog
