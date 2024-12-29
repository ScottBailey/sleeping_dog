#include <sb/sleeping_dog/router.hpp>
#include <router_data.hpp>

#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <iostream> // debugging only, remove

#include <boost/config.hpp>


namespace sb::sleeping_dog {


namespace {

auth::return_type default_auth(const request_type&) {
  return {true, response_type{}};
}

bool path_empty(const path_type& l) {
  return l.empty() || (l == "/");
}

/// @return If path is empty, retunr '/'; otherwise ensure path beings with '/' and ends without '/'.
path_type path_normalize(path_type path) {
  // if it's empty return '/'
  if (path_empty(path)) {
    return "/";
  }
  // maybe remove trailing
  if (path.back() == '/') {
    path.pop_back();
  }
  // maybe insert leading
  auto iter = path.begin();
  if (*iter != '/') {
    path.insert(iter,'/');
  }
  // ...and return
  return path;
}

// l & r must already be normalized
path_type path_combine(const path_type& l, const path_type& r) {
  if (path_empty(l)) {
    return r;
  }
  if (path_empty(r)) {
    return l;
  }
  return l + r;
}

// everything must already be normalized
path_type path_replace_front(path_type path, path_type old_prefix, path_type new_prefix) {

  if(old_prefix == new_prefix) {
    return path;
  }

  // ensure that old_prefix is at the front.
  if (path.find(old_prefix) != 0) {
    auto msg = fmt::format("path_replace_front('{}', '{}', '{}') - arg 1 doesn't begin with arg 2");
    spdlog::error(msg);
    throw std::runtime_error(msg);
  }

  // replace
  path.replace(0,old_prefix.size(),new_prefix);

  return path;
}

}


// ---

router::router()
  : auth_{ default_auth }
  , route_{ std::make_unique<router_data>() }
{
}


router::~router() = default;


//---

void router::add(verb_type verb, path_type path, callback_type cb) {
  // We normalize and combine the paths here:
  path = path_normalize(path);
  path = path_combine(prefix_,path);
  route_->map.emplace(std::make_pair(std::make_pair(verb,path),cb));
}


void router::auth(auth::callback_type cb) {
  auth_ = std::move(cb);
}


void router::drop(verb_type verb, path_type path)
{
  // We could normalize and combine paths here:
  path = path_normalize(path);
  path = path_combine(prefix_,path);
  route_->map.erase({verb,path});
}


response_type router::handle(const request_type& req) {

  auto [valid, resp] = auth_(req);
  if (!valid) {
    return resp;
  }

  auto verb = req.method();
  auto path = req.target();

  std::cout << verb << ": " << path << std::endl;

  auto iter = route_->map.find({verb,path});
  if (iter != route_->map.end()) {
    return iter->second(req);
  }

  std::cout << "resource not found" << std::endl;
  // this should actually be a resource not found or something like that.

  return response_type{};
}


path_type router::prefix() const {
  return prefix_;
}


void router::prefix(path_type new_prefix) {
  // normalize the inbound value.
  new_prefix = path_normalize(new_prefix);

  // same? just return
  if (prefix_ == new_prefix) {
    return;
  }

  // more sanity checking might be nice here?
  // sanitiy_check(new_prefix);

  auto replacement_route = std::make_unique<router_data>();
  for (auto& a : route_->map) {
    const path_type& old_path = a.first.second;
    const path_type new_path = path_replace_front(old_path,prefix_,new_prefix);
    // move the callback to the new location
    replacement_route->map.emplace(std::make_pair(std::make_pair(a.first.first,new_path),std::move(a.second)));
  }


  prefix_ = new_prefix;
  route_ = std::move(replacement_route);
}


} // namespace sb::sleeping_dog
