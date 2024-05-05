#include "Graph.hpp"

Graph::Graph() {}
void Graph::addNode(Node const &node) { nodes_[node.node()] = node; }
void Graph::addArc(Arc const &arc) { arcs_[arc.arc()] = arc; }

void Graph::build_master_graph() noexcept {
  for (auto const &[arc_id, arc] : arcs_) {
    adjacency_list_[arc.from()].push_back(arc.arc());
  }
}

std::optional<Arcs> Graph::operator[](nodeId id) const noexcept {
  auto arcs = adjacency_list_.find(id);
  if (arcs != adjacency_list_.end()) {
    for (auto arc : arcs->second) {
    }
    return arcs->second;
  }
  return std::nullopt;
}

Node const &Graph::node(nodeId id) const {
  auto node = nodes_.find(id);
  if (node != nodes_.end()) {
    return node->second;
  }
  throw std::runtime_error("Unknown node id in graph: " + std::to_string(id));
}

Arc const &Graph::arc(arcId id) const {
  auto arc = arcs_.find(id);
  if (arc != arcs_.end()) {
    return arc->second;
  }
  throw std::runtime_error("Unknown arc id in graph: " + std::to_string(id));
}
