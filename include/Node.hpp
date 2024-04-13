#pragma once

#include "NodeData.hpp"
#include <memory>

class Node {
public:
  Node() = default;

  template <typename... Args>
  Node(std::size_t id, Args... args) : id_(id), data_(std::make_shared(std::forward<Args>(args)...)) {}
  Node(std::size_t id, double lat, double lon, std::string const &name,
       std::set<Facility> const &facilities, std::string const &address)
      : id_(id), data_(std::make_unique<NodeData>(lat, lon, name, facilities)) {
  }

  std::size_t node() const { return id_; }
  std::shared_ptr<NodeData> data() { return data_; }

private:
  std::size_t id_;
  std::shared_ptr<NodeData> data_;
};