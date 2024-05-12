#include "DataLoader.hpp"
#include "Dijkstra.hpp"
#include "Graph.hpp"
#include "Node.hpp"
#include "Request.hpp"
#include "Response.hpp"

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;
using json = nlohmann::json;

int main() {
  Graph graph;
  DataLoader::load(graph);

  // Запуск інтерфейсу вводу-виводу boost::asio
  net::io_context ioc{1};

  // Створення TCP-слухача на порту 8080
  tcp::acceptor acceptor{ioc, {{}, 8080}};
  std::cout << "Server is ready" << std::endl;

  while (true) {
    // Очікування та обробка вхідного з'єднання
    tcp::socket socket{ioc};
    acceptor.accept(socket);

    // Отримання запиту від клієнта
    beast::flat_buffer buffer;
    http::request<http::string_body> request;
    http::read(socket, buffer, request);
    std::string data = request.body();

    json result_json;

    if (data.empty()) {
      std::cout << "Request is empty" << std::endl;
      // Відправлення відповіді клієнту
      auto response = Response::withPayload(result_json, request);
      http::write(socket, response);
      continue;
    }

    // Отримання параметра з запиту
    auto parsedRequest = Request::fromJsonString(data);
    std::cout << parsedRequest;

    auto closest = Dijkstra::find_closed_node(
        graph, parsedRequest.getLatitude(), parsedRequest.getLongitude());

    if (!closest) {
      std::cout << "Closest node not found. Graph is empty" << std::endl;
      // Відправлення відповіді клієнту
      auto response = Response::withPayload(result_json, request);
      http::write(socket, response);
      continue;
    }

    std::cout << "closest=" << *closest << std::endl;
    result_json = Dijkstra::optimize(graph, *closest, 1654330486);
    std::cout << result_json << std::endl;

    // Відправлення відповіді клієнту
    auto response = Response::withPayload(result_json, request);
    http::write(socket, response);
  }

  return 0;
}
