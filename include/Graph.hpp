#pragma once

#include "Arc.hpp"
#include "Node.hpp"
#include <iostream>
#include <map>
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

  std::vector<Node> &nodes() { return nodes_; }
  std::vector<Arc> &arcs() { return arcs_; }
  std::map<nodeId, Arcs> const &graph() { return adjacency_list_; }

  void build_master_graph();

private:
  std::map<nodeId, Arcs> adjacency_list_;
  std::vector<Node> nodes_;
  std::vector<Arc> arcs_;
};
