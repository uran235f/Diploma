#pragma once

#include "Transport.hpp"
#include <memory>

class Arc {
public:
  Arc() = default;

  template <typename... Args>
  Arc(std::size_t id, std::size_t from_node, std::size_t to_node,
      std::string const &street, double distance, Args... args)
      : id_(id), from_node_(from_node), to_node_(to_node), street_(street),
        distance_(distance),
        transport_(std::make_shared(std::forward<Args>(args)...)) {}
  Arc(std::size_t id, std::size_t from_node, std::size_t to_node,
      std::string const &street, double distance, std::string const &name,
      std::set<TransportType> const &types)
      : id_(id), from_node_(from_node), to_node_(to_node), street_(street),
        distance_(distance),
        transport_(std::make_shared<Transport>(name, types)) {}

  std::size_t arc() const { return id_; }
  std::shared_ptr<Transport> transportData() const { return transport_; }

  friend std::ostream &operator<<(std::ostream &os, Arc const &arc) {
    std::ostringstream oss;
    oss << "Arc: id=" << arc.id_ << " from=" << arc.from_node_
        << " to=" << arc.to_node_ << " distance=" << arc.distance_
        << " street=" << arc.street_ << " " << *arc.transport_;
    return os << oss.str();
  }

private:
  std::size_t id_;
  std::size_t from_node_;
  std::size_t to_node_;
  std::string street_;
  double distance_;
  std::shared_ptr<Transport> transport_;
};