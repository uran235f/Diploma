#pragma once

#include "MetaData.hpp"
#include <memory>

class Node {
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

private:
  std::size_t id_;
  std::shared_ptr<MetaData> data_;
};