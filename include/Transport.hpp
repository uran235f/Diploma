#pragma once

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/level_enum.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/string.hpp>
#include <set>
#include <sstream>
#include <string>

enum class TransportType { bus, subway, car, pedestrian };

class Transport {
public:
  Transport() = default;
  Transport(std::string const &name, std::set<TransportType> const &types)
      : name_(name), transportTypes_(types) {}

  friend std::ostream &operator<<(std::ostream &os, Transport const &t) {
    std::ostringstream oss;
    oss << "transport: name=" << t.name_ << " types=[";
    for (auto it = t.transportTypes_.cbegin(); it != t.transportTypes_.cend();
         ++it) {
      oss << static_cast<int>(*it);
      if (std::next(it) != t.transportTypes_.cend()) {
        oss << ",";
      }
    }
    oss << "]";

    return os << oss.str();
  }

  std::string const &name() const { return name_; }
  std::set<TransportType> const &transport() const { return transportTypes_; }

  template <class Archive>
  void save(Archive &ar, const unsigned int version) const;
  template <class Archive> void load(Archive &ar, const unsigned int version);

  BOOST_SERIALIZATION_SPLIT_MEMBER();

private:
  std::string name_;
  std::set<TransportType> transportTypes_;
};

template <class Archive>
void Transport::save(Archive &ar, const unsigned int version) const {
  ar &name_ &transportTypes_;
}

template <class Archive>
void Transport::load(Archive &ar, const unsigned int version) {
  ar &name_ &transportTypes_;
}