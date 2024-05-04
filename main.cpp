#include "DataLoader.hpp"
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
  auto loader = DataLoader();
  loader.loadNodes(graph);
  loader.loadArcs(graph);
  std::cout << "Nodes size=" << graph.nodes().size() << std::endl;
  std::cout << graph.nodes()[0] << std::endl;
  std::cout << "Arcs size=" << graph.arcs().size() << std::endl;
  std::cout << graph.arcs()[0] << std::endl;
  graph.build_master_graph();

  auto it = graph.graph().find(0);
  if (it != graph.graph().end()) {
    std::cout << "Arcs in node 0 - " << it->second.size() << std::endl;
    for (auto const &arc : it->second) {
      std::cout << "Arcid " << arc << std::endl;
      std::cout << "from node 0 -> " << graph.arcs()[arc] << std::endl;
    }
  } else {
    std::cout << "No node 0" << std::endl;
  }

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

    response.body() = "Received data " + data;
    response.prepare_payload();

    // Відправлення відповіді клієнту
    http::write(socket, response);
  }

  return 0;
}
