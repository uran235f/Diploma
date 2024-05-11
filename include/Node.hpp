#pragma once

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <memory>

#include "MetaData.hpp"

class Node {
  friend class boost::serialization::access;

public:
  Node() = default;

  template <typename... Args>
  Node(std::size_t id, Args... args)
      : id_(id),
        data_(std::make_shared<MetaData>(std::forward<Args>(args)...)) {}
  Node(std::size_t id, double lat, double lon, std::string const &name,
       std::string const &address, std::set<Facility> const &facilities)
      : id_(id),
        data_(std::make_shared<MetaData>(lat, lon, name, address, facilities)) {
  }

  std::size_t node() const { return id_; }
  std::shared_ptr<MetaData> data() const { return data_; }

  friend std::ostream &operator<<(std::ostream &os, Node const &node) {
    std::ostringstream oss;
    oss << "Node: id=" << node.id_ << " " << *node.data_;
    return os << oss.str();
  }

  template <class Archive>
  void save(Archive &ar, const unsigned int version) const;
  template <class Archive> void load(Archive &ar, const unsigned int version);

  BOOST_SERIALIZATION_SPLIT_MEMBER();

private:
  std::size_t id_;
  std::shared_ptr<MetaData> data_;
};

template <class Archive>
void Node::save(Archive &ar, const unsigned int version) const {
  ar &id_ &data_;
}

template <class Archive>
void Node::load(Archive &ar, const unsigned int version) {
  ar &id_ &data_;
}
