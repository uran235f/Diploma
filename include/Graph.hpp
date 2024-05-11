#pragma once

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <iostream>
#include <map>
#include <optional>
#include <vector>

#include "Arc.hpp"
#include "Node.hpp"

using arcId = std::size_t;
using nodeId = std::size_t;
using Arcs = std::vector<arcId>;

class Graph {
  friend class boost::serialization::access;

public:
  explicit Graph();
  ~Graph(){};

  void addNode(Node const &node);
  void addArc(Arc const &arc);

  std::map<nodeId, Arcs> const &graph() const { return adjacency_list_; }
  std::map<nodeId, Node> const &nodes() const { return nodes_; }
  std::map<arcId, Arc> const &arcs() const { return arcs_; }
  Node const &node(nodeId id) const;
  Arc const &arc(arcId id) const;

  void build_master_graph() noexcept;
  std::optional<Arcs> operator[](nodeId id) const noexcept;

  template <class Archive>
  void save(Archive &ar, const unsigned int version) const;
  template <class Archive> void load(Archive &ar, const unsigned int version);

  BOOST_SERIALIZATION_SPLIT_MEMBER();

private:
  std::map<nodeId, Arcs> adjacency_list_;
  std::map<nodeId, Node> nodes_;
  std::map<arcId, Arc> arcs_;
};

template <class Archive>
void Graph::save(Archive &ar, const unsigned int version) const {
  ar &adjacency_list_ &nodes_ &arcs_;
}

template <class Archive>
void Graph::load(Archive &ar, const unsigned int version) {
  ar &adjacency_list_ &nodes_ &arcs_;
}
