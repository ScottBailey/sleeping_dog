#pragma once

// I'm not sure how expensive including these are, but it could be worthwhile.
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/verb.hpp>
#include <boost/beast/http/status.hpp>

namespace sb::sleeping_dog {

using request_type = boost::beast::http::request<boost::beast::http::string_body>;
using response_type = boost::beast::http::response<boost::beast::http::string_body>;
using verb_type = boost::beast::http::verb;
using status_type = boost::beast::http::status;
using field_type = boost::beast::http::field;
using path_type = std::string;  // This *could* become a std::filesystem::path type?

} // namespace sb::sleeping_dog


namespace sb::sleeping_dog::auth {

struct return_type {
  bool authorized = false;
  response_type response = response_type{};
};
using callback_type = std::function<return_type(const request_type&)>;

} // namespace sb::sleeping_dog::auth
