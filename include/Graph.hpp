#pragma once

#include "Arc.hpp"
#include "Node.hpp"
#include <iostream>
#include <vector>

class Graph {
public:
  explicit Graph();
  ~Graph(){};

  void addNode(Node const &node);
  void addArc(Arc const &arc);

  std::vector<Node> &nodes() { return nodes_; }
  std::vector<Arc> &arcs() { return arcs_; }

private:
  std::vector<Node> nodes_;
  std::vector<Arc> arcs_;
};