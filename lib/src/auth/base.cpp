#include <sb/sleeping_dog/auth/base.hpp>

namespace sb::sleeping_dog::auth {

base::base() = default;
base::~base() = default;


return_type base::authorize(const request_type& req) {
  // test for bearer token...


  // or call internal
  return i_authorize(req);
}


} // namespace sb::sleeping_dog::auth
