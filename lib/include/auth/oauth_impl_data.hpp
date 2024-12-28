#pragma once

namespace sb::sleeping_dog::auth {

struct oauth::impl_data {

  impl_data(boost::urls::url auth_url, std::string_view client_id, std::string_view client_secret)
    : auth_url_ {auth_url}
    , client_id_ {client_id}
    , client_secret_ {client_secret}
  {}

  boost::urls::url auth_url_;
  std::string client_id_;
  std::string client_secret_;

  std::string redirect_uri_;
  std::string redirect_uri_target_ = "/authcode";

};

} // namespace sb::sleeping_dog::auth
