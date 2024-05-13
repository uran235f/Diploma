#pragma once

#include "Graph.hpp"
#include "Request.hpp"

#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

struct Item {
  std::size_t node_id;
  std::optional<std::size_t> parent_arc;
  std::optional<TransportType> transport;
  std::shared_ptr<Item> parent;
  double distance;

  bool operator>(Item const &rhs) const {
    return this->distance > rhs.distance;
  }

  bool operator<(Item const &rhs) const {
    return this->distance < rhs.distance;
  }

  bool operator==(Item const &rhs) const {
    return this->node_id == rhs.node_id && this->parent_arc.value() &&
           rhs.parent_arc.value();
  }

  friend std::ostream &operator<<(std::ostream &os, Item const &item) {
    std::ostringstream oss;
    oss << "Item: node " << item.node_id;
    if (item.parent_arc) {
      oss << " via " << *item.parent_arc << "["
          << static_cast<int>(*item.transport) << "]";
    } else {
      oss << " via none";
    }
    return os << oss.str();
  }

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
  static nlohmann::json reduce_result(nlohmann::json &modifiable);
  static std::optional<nodeId> bfs(Graph const &g,
                                   std::optional<nodeId> const &from,
                                   int requested_facility);
};