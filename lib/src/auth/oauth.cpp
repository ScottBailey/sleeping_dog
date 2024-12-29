#include <sb/sleeping_dog/auth/oauth.hpp>
#include <auth/oauth_impl.hpp>

#include <boost/url.hpp>



namespace sb::sleeping_dog::auth {


oauth::oauth(private_key, boost::urls::url auth_url, std::string_view client_id, std::string_view client_secret)
  : impl_{ std::make_unique<oauth_impl>( auth_url, client_id, client_secret ) }
{
}


oauth::~oauth() = default;

//---


ptr oauth::create(std::string_view auth_url_str, std::string_view client_id, std::string_view client_secret) {

  // Parse the authorization endpoint URL
  auto result = boost::urls::parse_uri(auth_url_str);
  if (!result) {
    //throw std::runtime_error(fmt::format("Error parsing authorization endpoint: {}", result.error()));
    throw std::runtime_error("Error parsing authorization endpoint: ***fix**");
  }
  boost::urls::url url = result.value();
  return std::make_shared<oauth>(private_key{}, url, client_id, client_secret);
}


return_type oauth::authorize(const request_type& req) {
  return impl_->authorize(req);
}


} // namespace sb::sleeping_dog::auth
