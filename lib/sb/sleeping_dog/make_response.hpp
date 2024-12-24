#pragma once

#include <sb/sleeping_dog/types.hpp>


namespace sb::sleeping_dog {

response_type make_response(const request_type& req, std::string_view body, status_type status = status_type::ok);

response_type bad_request(const request_type& req, std::string_view msg);
response_type not_found(const request_type& req, std::string_view target);
response_type server_error(const request_type& req, std::string_view what);

}
