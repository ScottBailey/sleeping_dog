#pragma once

#include <sb/sleeping_dog/auth/base.hpp>
#include <boost/url/url.hpp>


namespace sb::sleeping_dog::auth {

class oauth
  : public base
  , public std::enable_shared_from_this<base>
{
  struct private_key {};
public:
  oauth(private_key, boost::urls::url auth_url, std::string_view client_id, std::string_view client_secret);
  virtual ~oauth();

  static ptr create(std::string_view auth_url, std::string_view client_id, std::string_view client_secret);

  virtual return_type authorize(const request_type&) override;


private:
  struct impl_data;  // should scope this
  std::unique_ptr<impl_data> impl_;
};

} // namespace sb::sleeping_dog::auth
