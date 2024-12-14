#include <server_impl.hpp>
#include <sb/sleeping_dog/router.hpp>

namespace sb::sleeping_dog {

server_impl::server_impl(private_token /*key*/) {
}


server_impl::~server_impl() = default;

//--- alphabetic -------------------------------------------------------------------------------------------------------

std::shared_ptr<server_impl> server_impl::create() {
  return std::make_shared<server_impl>(private_token{});
}


void server_impl::join() {
}


void server_impl::listen(unsigned port) {
  listen("0.0.0.0",port);
}


void server_impl::listen(std::string_view /*addr*/, unsigned /*port*/) {
}


sleeping_dog::router* server_impl::router() {
    // throws smth if router is nullptr
  if (!router_)
    throw "smth";

  return router_.get();
}


void server_impl::run() {
}

  /// non-blocking call
void server_impl::start() {
}


void server_impl::take(std::unique_ptr<sleeping_dog::router>&& r) {
  router_ = std::move(r);
}


bool server_impl::try_join() {
  return true;
}


} // namespace sb::sleeping_dog
