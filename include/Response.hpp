#pragma once

#include <boost/beast/http.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;
using json = nlohmann::json;

class Response {
public:
  static http::response<http::string_body>
  withPayload(json const &json,
              http::request<http::string_body> const &request) {
    http::response<http::string_body> response{http::status::ok,
                                               request.version()};
    response.set(http::field::content_type, "text/plain");
    response.set(http::field::access_control_allow_origin, "*");
    response.set(http::field::access_control_allow_headers, "Content-Type");
    response.keep_alive(request.keep_alive());
    response.body() = json.empty() || json.is_null() ? "" : to_string(json);
    response.prepare_payload();
    return response;
  }

private:
  Response() = default;
};
