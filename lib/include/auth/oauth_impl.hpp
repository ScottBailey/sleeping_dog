#pragma once

#include <sb/sleeping_dog/types.hpp>

#include <string_view>
#include <boost/url/url.hpp>
#include <boost/sqlite/connection.hpp>
#include <boost/sqlite/statement.hpp>


namespace sb::sleeping_dog::auth {

class oauth_impl {
public:
  oauth_impl(boost::urls::url auth_url, std::string_view client_id, std::string_view client_secret);
  ~oauth_impl();

  return_type authorize(const request_type&);

private:
  boost::sqlite::connection db_;
  boost::sqlite::statement s1_;


  boost::urls::url auth_url_;
  std::string client_id_;
  std::string client_secret_;

  std::string redirect_uri_;
  std::string redirect_uri_target_ = "/authcode";

};

} // namespace sb::sleeping_dog::auth
