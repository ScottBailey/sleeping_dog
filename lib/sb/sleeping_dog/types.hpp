#pragma once

// I'm not sure how expensive including these are, but it could be worthwhile.
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/verb.hpp>


namespace sb::sleeping_dog {

using request_type = boost::beast::http::request<boost::beast::http::string_body>;
using response_type = boost::beast::http::response<boost::beast::http::string_body>;
using verb_type = boost::beast::http::verb;
using path_type = std::string;

} // namespace sb::sleeping_dog
