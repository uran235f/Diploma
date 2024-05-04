#pragma once

#include "Arc.hpp"
#include "Node.hpp"
#include <iostream>
#include <map>
#include <optional>
#include <vector>

using arcId = std::size_t;
using nodeId = std::size_t;
using Arcs = std::vector<arcId>;

class Graph {
public:
  explicit Graph();
  ~Graph(){};

  void addNode(Node const &node);
  void addArc(Arc const &arc);

  std::vector<Node> const &nodes() const { return nodes_; }
  std::vector<Arc> const &arcs() const { return arcs_; }
  Node const &node(nodeId id) const { return nodes_[id]; }
  Arc const &arc(arcId id) const { return arcs_[id]; }
  std::map<nodeId, Arcs> const &graph() const { return adjacency_list_; }

  void build_master_graph() noexcept;
  std::optional<Arcs> operator[](nodeId id) const noexcept;

private:
  std::map<nodeId, Arcs> adjacency_list_;
  std::vector<Node> nodes_;
  std::vector<Arc> arcs_;
};
