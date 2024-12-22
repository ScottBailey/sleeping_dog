#include <sb/sleeping_dog/router.hpp>
#include <router_data.hpp>


namespace sb::sleeping_dog {

router::router()
  : data_{ std::make_unique<router_data>() }
{
}


router::~router() = default;

void router::add(verb_type verb, path_type path, callback_type cb) {

  data_->map.emplace(std::make_pair(std::make_pair(verb,path),cb));

}


void router::auth(auth_type) {
}


void router::drop(verb_type verb, path_type path) {
  data_->map.erase({verb,path});
}


response_type router::handle(const request_type& req) {

  // we should do something to deal with authorization here


  auto verb = req.method();
  auto path = req.target();

  auto iter = data_->map.find({verb,path});
  if (iter != data_->map.end()) {
    return iter->second(req);
  }

  // this should actually be a resource not found or something like that.

  return response_type{};
}


void router::prefix(path_type) {
}


} // namespace sb::sleeping_dog
