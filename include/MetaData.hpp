#pragma once

#include <set>
#include <sstream>
#include <string>

enum class Facility {
  Cardiology,
  Emergency,
  General,
  Gynaecology,
  Orthopaedics,
  Paediatrics,
  Stomatology,
  Trauma,
  Community,
  Radiology,
  Transplant,
  Urology,
  Hospital,
  Birthing_centre,
  Centre,
  Clinic,
  Dentist,
  Doctor,
  Laboratory,
  Pharmacy,
  Psychiatry,
  Orthodontics
};

class MetaData {
public:
  MetaData() = default;
  MetaData(double lat, double lon, std::string const &name,
           std::string const &address, std::set<Facility> const &facilities)
      : lat_(lat), lon_(lon), name_(name), address_(address),
        facilities_(facilities) {}

  friend std::ostream &operator<<(std::ostream &os, MetaData const &data) {
    std::ostringstream oss;
    oss << "Metadata: lat=" << std::to_string(data.lat_) << " lon=" << data.lon_
        << " name=" << data.name_ << " address=" << data.address_
        << " facilities=[";

    for (auto it = data.facilities_.cbegin(); it != data.facilities_.cend();
         ++it) {
      oss << static_cast<int>(*it);
      if (std::next(it) != data.facilities_.cend()) {
        oss << ",";
      }
    }
    oss << "]";

    return os << oss.str();
  }

  double lat() const { return lat_; }
  double lon() const { return lon_; }
  std::string const &name() const { return name_; }
  std::string const &address() const { return address_; }
  std::set<Facility> const &facilities() const { return facilities_; }

private:
  double lat_;
  double lon_;
  std::string name_;
  std::string address_;
  std::set<Facility> facilities_;
};