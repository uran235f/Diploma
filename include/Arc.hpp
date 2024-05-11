#pragma once

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/string.hpp>
#include <memory>

#include "Transport.hpp"

class Arc {
  friend class boost::serialization::access;

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

  std::size_t from() const { return from_node_; }
  std::size_t to() const { return to_node_; }
  double distance() const { return distance_; }

  template <class Archive>
  void save(Archive &ar, const unsigned int version) const;
  template <class Archive> void load(Archive &ar, const unsigned int version);

  BOOST_SERIALIZATION_SPLIT_MEMBER();

private:
  std::size_t id_;
  std::size_t from_node_;
  std::size_t to_node_;
  std::string street_;
  double distance_;
  std::shared_ptr<Transport> transport_;
};

template <class Archive>
void Arc::save(Archive &ar, const unsigned int version) const {
  ar &id_ &from_node_ &to_node_ &street_ &distance_ &transport_;
}

template <class Archive>
void Arc::load(Archive &ar, const unsigned int version) {
  ar &id_ &from_node_ &to_node_ &street_ &distance_ &transport_;
}