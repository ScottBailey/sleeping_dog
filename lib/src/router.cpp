#include <sb/sleeping_dog/router.hpp>
#include <router_impl.hpp>


namespace sb::sleeping_dog {

router::router()
  : impl_{ std::make_unique<router_impl>() }
{
}


router::~router() = default;

void router::add(request_type, path_type, callback_type) {
}


void router::auth(auth_type) {
}


void router::drop(request_type, path_type) {
}


void router::handle(request_type, path_type, payload_type) {
}


void router::prefix(path_type) {
}


} // namespace sb::sleeping_dog
