#pragma once

#include <memory>
#include <sb/sleeping_dog/types.hpp>


namespace sb::sleeping_dog::auth {



class base
{
protected:
  base();
public:
  virtual ~base();
  // -- authorize MAY require scope, such as "read" or "write"?
  // scope might be best managed as a bit field?
  return_type authorize(const request_type& req);
protected:
  virtual return_type i_authorize(const request_type& req) = 0;
};

using ptr = std::shared_ptr<base>;

template <typename ROUTER>
void connect(ptr& p, ROUTER& r) {
  std::weak_ptr<base> weak_ptr = p;
  r.auth([weak_ptr](const request_type& req) -> return_type {
    auto shared = weak_ptr.lock();
    if (!shared)
      return {};
    return shared->authorize(req);
  });
}

} // namespace sb::sleeping_dog::auth
