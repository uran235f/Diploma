#include "Graph.hpp"

Graph::Graph() {}
void Graph::addNode(Node const &node) { nodes_.push_back(node); }
void Graph::addArc(Arc const &arc) { arcs_.push_back(arc); }

void Graph::build_master_graph() noexcept {
  for (auto const &arc : arcs_) {
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