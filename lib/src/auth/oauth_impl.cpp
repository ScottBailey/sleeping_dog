#include <auth/oauth_impl.hpp>
#include <boost/url.hpp>
#include <fmt/format.h>


#include <boost/beast/http.hpp> // debug remove!!!
#include <iostream> // debug remove!!!


namespace sb::sleeping_dog::auth {

namespace {

void dump(const request_type& r) {

  std::cout << "------------------------------------------------\n";

  std::cout << r ;
  //  for (auto i = r.begin(); i != r.end(); ++i) {
  //    std::cout << i << "\n";
  //}

  std::cout << "------------------------------------------------\n";

} // dump()

} // namespace

//---

oauth_impl::oauth_impl(boost::urls::url auth_url, std::string_view client_id, std::string_view client_secret)
  : auth_url_ {auth_url}
  , client_id_ {client_id}
  , client_secret_ {client_secret}
{
}


oauth_impl::~oauth_impl() = default;


//---

return_type oauth_impl::authorize(const request_type& req) {

  // If this seems to be the result of
//****************************************

  // If authorization code exists...
  if (false) {
    // ...and matches scope, return true
    if (true) {
      return return_type{true};
    }

    // Scope mismatch, let the user know.
    return return_type{false};  // <--- needs to be an error
  }

  // The user needs to authorize, generate the redirect response here:

  dump(req);

  // Either use an existing redirect_uri_ or parse from the existing request.
  std::string redirect_uri = redirect_uri_;
  if (redirect_uri.empty()) {
    auto host_iter = req.find(field_type::host);
    if (host_iter == req.end()) {
      return return_type{false}; // <--- needs to be an error
    }
    redirect_uri = fmt::format( "https://{}", host_iter->value());
  }
  redirect_uri += redirect_uri_target_;




  const auto state = std::string("random_state_string"); // this is *NOT* the way. Needs to be randomly generated and probably cached.

  // Initialize URL builder with the pre-existing auth_url_
  auto redirect_url = auth_url_;
  redirect_url.set_params( {
        {"response_type", "code"},
        {"client_id", client_id_},
        {"scope", "openid email"},
        {"redirect_uri", redirect_uri},
        {"state", state},
        //{"grant_type", "authorization_code"},
        //{"device_id", "01"},
        //{"device_name", "puppy"}
      } );

  std::cout << "-------------> " << redirect_url << "\n";

  auto rt = return_type{false, {status_type::found, req.version()}};
  rt.response.set(field_type::location, redirect_url.c_str());    /// <<----------- not sure c_str() is the right call.
  rt.response.set(field_type::content_type, "text/html");
  rt.response.keep_alive(req.keep_alive());
  rt.response.prepare_payload();

  return rt;
}


} // namespace sb::sleeping_dog::auth
