#pragma once

namespace sb::sleeping_dog {

class connection;

/// @see https://en.wikipedia.org/wiki/HTTP#Request_methods
enum class request {
  get,
  post,

};

//using request = boost::beast::http::request<boost::beast::http::string_body>;
//using response = boost::beast::http::response<boost::beast::http::string_body>;
//using get_callback = std::function<void(request, response)>;

/// what about this?
using get_callback = std::function<void(json,err)>;






} // namespace sleeping_dog
