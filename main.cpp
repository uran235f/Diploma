#include "DataLoader.hpp"
#include "Dijkstra.hpp"
#include "Graph.hpp"
#include "Node.hpp"
#include "Request.hpp"

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

int main() {
  Graph graph;
  auto loader = std::make_unique<DataLoader>();
  loader->loadNodes(graph);
  loader->loadArcs(graph);

  graph.build_master_graph();

  auto it = graph.graph().find(26201725);
  if (it != graph.graph().end()) {
    std::cout << "Arcs in node 0 - " << it->second.size() << std::endl;
    for (auto const &arc : it->second) {
      std::cout << "Arcid " << arc << std::endl;
      std::cout << "from node 0 -> " << graph.arcs().at(arc) << std::endl;
    }
  } else {
    std::cout << "No node 0" << std::endl;
  }

  auto result_json = Dijkstra::optimize(graph, 249499947, 5965985171);
  std::cout << result_json << std::endl;

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

    // Отримання параметра з запиту
    std::string data = request.body();
    if (!data.empty()) {
      std::cout << Request::fromJsonString(data);
    }

    // Створення відповіді сервера
    http::response<http::string_body> response{http::status::ok,
                                               request.version()};
    response.set(http::field::content_type, "text/plain");
    response.set(http::field::access_control_allow_origin,
                 "*"); // Додавання заголовка Access-Control-Allow-Origin
    response.set(
        http::field::access_control_allow_headers,
        "Content-Type"); // Додавання заголовка Access-Control-Allow-Headers
    response.keep_alive(request.keep_alive());

    auto a = 2 + 2;

    response.body() = result_json;
    response.prepare_payload();

    // Відправлення відповіді клієнту
    http::write(socket, response);
  }

  return 0;
}
