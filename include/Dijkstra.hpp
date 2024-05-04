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
};

class Dijkstra {
public:
  static std::string optimize(Graph const &g);
};