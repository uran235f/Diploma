#include "Graph.hpp"

Graph::Graph() {}
void Graph::addNode(Node const &node) { nodes_.push_back(node); }
void Graph::addArc(Arc const &arc) { arcs_.push_back(arc); }
