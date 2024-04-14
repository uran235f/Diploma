#pragma once
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

private:
  std::string name_;
  std::set<TransportType> transportTypes_;
};