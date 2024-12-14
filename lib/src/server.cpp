#include <sb/sleeping_dog/server.hpp>
#include <server_impl.hpp>

namespace sb::sleeping_dog {

//--- constructors/destructor ------------------------------------------------------------------------------------------

server::server()
  : impl_{ server_impl::create() }
{
}

server::~server() {
}


//--- alphabetic -------------------------------------------------------------------------------------------------------

void server::join() {
  impl_->join();
}


void server::listen(unsigned port) {
  impl_->listen(port);
}


void server::listen(std::string_view addr, unsigned port) {
  impl_->listen(addr,port);
}


router* server::router() {
  return impl_->router();
}


void server::run() {
  impl_->run();
}


void server::start() {
  impl_->start();
}


void server::take(std::unique_ptr<sleeping_dog::router>&& rtr) {
  impl_->take(std::move(rtr));
}


bool server::try_join() {
  return impl_->try_join();
}











} // namespace sb::sleeping_dog
