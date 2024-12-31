#include <auth/oauth_impl.hpp>
#include <boost/url.hpp>
#include <fmt/format.h>


#include <boost/beast/http.hpp> // debug remove!!!
#include <iostream> // debug remove!!!


namespace sb::sleeping_dog::auth {

namespace {

void dump(const request_type& r) {

  std::cout << "------------------------------------------------\n";

  std::cout << "Target: " << r.target() << "\n";

  std::cout << r ;
  //  for (auto i = r.begin(); i != r.end(); ++i) {
  //    std::cout << i << "\n";
  //}

  std::cout << "------------------------------------------------\n";

} // dump()

} // namespace

//---

oauth_impl::oauth_impl(boost::urls::url auth_url, std::string_view client_id, std::string_view client_secret)
  : db_(boost::sqlite::in_memory)
  , auth_url_ {auth_url}
  , client_id_ {client_id}
  , client_secret_ {client_secret}
{
  db_.execute
    (
     "CREATE TABLE IF NOT EXISTS state_table ("
     " time       intger not null,"
     " source     integer);"
     );


  s1_ = db_.prepare("SELECT * FROM state_table WHERE time = ? and source = ?;");
}


oauth_impl::~oauth_impl() = default;


//---

return_type oauth_impl::i_authorize(const request_type& req) {


  dump(req);

  // Evaluate if this is a response
  // auto target = req.target();
  // if (target has redirect_uri_target_ ) {
  //  evaluate if this is success or fail?
  // }
  // evaluate if the target has an auth token? <-- should this happen in the base? Yes, I think it should.

  boost::urls::url original_url;
  try {
    auto original_host = req.find("host")->value();
    auto original_target = req.target();
    original_url = boost::urls::parse_uri(fmt::format("https://{}{}", original_host, original_target)).value();
  }
  catch (boost::system::system_error &e) {
    // this should actually return the reason as {action::send_resp, response with reason}
    return {};
  }

  //std::cout << "test: " << original_url.encoded_path() << " - " << redirect_uri_target_ << "\n";

  if (original_url.encoded_path() == redirect_uri_target_) {

    // We have 2 choices here, either redirect the user one more time, or modify the request.
    // FOR THIS MOMENT, we are going to redirect. It seems more likely that what we actually want to do is update the target and continue.

    // find state from original_url
    std::string state_str;
    for (auto a : original_url.params()) {
      if (a->key != "state")
        continue;
      state_str = a->value;
      break;
    }

    if (state_str.empty()) {
      // this should actually return the reason as {action::send_resp, response with reason}
      return {};
    }

    // Create the token here?

    auto redirect_url = fmt::format( "https://{}{}", original_url.encoded_host_and_port(), state_str);

    auto rt = return_type{action::respond_now, {status_type::found, req.version()}};
    rt.response.set(field_type::location, redirect_url.c_str());    /// <<----------- not sure c_str() is the right call.
    rt.response.set(field_type::content_type, "text/html");
    rt.response.keep_alive(req.keep_alive());
    rt.response.prepare_payload();

    return rt;
  }


  // The user needs to authorize, generate the redirect response here:


  // Either use an existing redirect_uri_ or parse from the existing request.
  std::string redirect_uri = redirect_uri_;
  if (redirect_uri.empty()) {
    redirect_uri = fmt::format( "https://{}", original_url.encoded_host_and_port());
  }
  redirect_uri += redirect_uri_target_;

  // We are going to store the original target into the state field
  const auto state = req.target();
  //const auto state = "random_state_string";

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

  std::cout << original_url << "\n";
  std::cout << "-------------> https://accounts.google.com/o/oauth2/auth?response_type=code&client_id=979731927289-ccbud3nflath42ebt5lj8otsl4pefra8.apps.googleusercontent.com&scope=openid%20email&redirect_uri=https://addoria.com:8080/authcode&state=random_state_string\n";
  std::cout << "-------------> " << redirect_url << "\n";

  auto rt = return_type{action::respond_now, {status_type::found, req.version()}};
  rt.response.set(field_type::location, redirect_url.c_str());    /// <<----------- not sure c_str() is the right call.
  rt.response.set(field_type::content_type, "text/html");
  rt.response.keep_alive(req.keep_alive());
  rt.response.prepare_payload();

  return rt;
}


} // namespace sb::sleeping_dog::auth
