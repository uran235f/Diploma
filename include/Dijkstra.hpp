#pragma once

#include "Graph.hpp"

#include <memory>
#include <optional>
#include <string>

struct Item {
  std::size_t node_id;
  std::optional<std::size_t> parent_arc;
  std::shared_ptr<Item> parent;
  double distance;

  bool operator>(Item const &rhs) const {
    return this->distance > rhs.distance;
  }

  friend std::ostream &operator<<(std::ostream &os, Item const &item) {
    std::ostringstream oss;
    oss << "Item: node " << item.node_id;
    if (item.parent_arc) {
      oss << " via " << *item.parent_arc;
    } else {
      oss << " via none";
    }
    return os << oss.str();
  }

  static void print_chain(Item const &item, Graph const &g);
};

class Dijkstra {
public:
  static std::string optimize(Graph const &g);

private:
  static std::string generate_json_result(Item const &item, Graph const &g,
                                          std::size_t start);
};