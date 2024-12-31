#pragma once

#include <sb/sleeping_dog/auth/base.hpp>
#include <boost/url/url.hpp>

#include <memory>


namespace sb::sleeping_dog::auth {

struct oauth_impl;

class oauth
  : public base
  , public std::enable_shared_from_this<base>
{
  struct private_key {};
public:
  oauth(private_key, boost::urls::url auth_url, std::string_view client_id, std::string_view client_secret);
  virtual ~oauth();

  static ptr create(std::string_view auth_url, std::string_view client_id, std::string_view client_secret);

protected:
  virtual return_type i_authorize(const request_type&) override;

private:
  std::unique_ptr<oauth_impl> impl_;
};

} // namespace sb::sleeping_dog::auth
