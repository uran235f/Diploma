#pragma once

#include "Graph.hpp"
#include "Request.hpp"

#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

struct Item {
  enum class Speeds { SUBWAY = 80, BUS = 40, CAR = 45, PERDESTRIAN = 5 };

  std::size_t node_id;
  std::optional<std::size_t> parent_arc;
  std::shared_ptr<Item> parent;
  TransportType transport;
  double distance;
  double time;

  Item() = default;
  Item(Item const &) = default;
  Item(Item &&) = default;
  Item &operator=(Item const &) = default;
  Item &operator=(Item &&) = default;
  Item(std::size_t node_id, std::optional<std::size_t> const &parent_arc,
       TransportType transport, std::shared_ptr<Item> parent, double distance)
      : node_id(node_id), parent_arc(parent_arc), transport(transport),
        parent(parent), distance(distance) {
    time = distance * 3600 / (transportToSpeed(transport) * 1000);
  }

  bool operator>(Item const &rhs) const { return this->time > rhs.time; }
  bool operator<(Item const &rhs) const { return this->time < rhs.time; }

  bool operator==(Item const &rhs) const {
    return this->node_id == rhs.node_id && this->parent_arc.value() &&
           rhs.parent_arc.value();
  }

  friend std::ostream &operator<<(std::ostream &os, Item const &item) {
    std::ostringstream oss;
    oss << "Item: node " << item.node_id;
    if (item.parent_arc) {
      oss << " via " << *item.parent_arc << "["
          << static_cast<int>(item.transport) << "]";
    } else {
      oss << " via none";
    }
    return os << oss.str();
  }

  static double transportToSpeed(TransportType transport);
  static void print_chain(Item const &item, Graph const &g);
};

class Dijkstra {
public:
  static std::string optimize(Graph const &g, Request const &request);
  static std::optional<nodeId> find_closed_node(Graph const &g, double lat,
                                                double lon);

private:
  static std::string generate_json_result(Item const &item, Graph const &g,
                                          std::size_t start);
  static void add_additional_fields(nlohmann::json &to, double dist);
  static std::optional<nodeId> bfs(Graph const &g,
                                   std::optional<nodeId> const &from,
                                   int requested_facility);
  static std::vector<TransportType>
  select_transport(std::set<TransportType> const &transport,
                   RequestedTransport requestedType);

  template <typename Containter>
  static void print_transport(std::string const &prepend,
                              Containter const &container);
};