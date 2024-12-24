#include <sb/sleeping_dog/make_response.hpp>
#include <fmt/format.h>

namespace sb::sleeping_dog {

response_type bad_request(const request_type& req, std::string_view msg) {
  return make_response(req, msg, status_type::bad_request);
}


response_type make_response(const request_type& req, std::string_view body, status_type status) {
  response_type rv{status, req.version()};
  rv.set(boost::beast::http::field::server, "sleeping dog 0.0.1");
  rv.set(boost::beast::http::field::content_type, "text/html");
  rv.keep_alive(req.keep_alive());
  rv.body() = std::string(body);
  rv.prepare_payload();
  return rv;
}


response_type not_found(const request_type& req, std::string_view target) {
  return make_response(req,
      fmt::format("Resource not found: '{}'", target),
      status_type::not_found);
}


response_type server_error(const request_type& req, std::string_view what) {
  return make_response(req,
      fmt::format("An error occurred: '{}'", what),
      status_type::internal_server_error);

}


}
