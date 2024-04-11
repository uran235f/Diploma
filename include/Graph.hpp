#pragma once

#include "Node.hpp"
#include <vector>

class Graph {
public:
  explicit Graph();
  ~Graph(){};

private:
  std::vector<Node> nodes_;
};