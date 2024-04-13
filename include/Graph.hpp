#pragma once

#include "Node.hpp"
#include <vector>
#include <iostream>

class Graph {
public:
  explicit Graph();
  ~Graph(){};

  void addNode(Node const &node);
  void showNodes() const {
    for (auto const &n : nodes_) {
      std::cout << n.node() << std::endl;
    }
  }

private:
  std::vector<Node> nodes_;
};