#pragma once
#include <set>
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

class NodeData {
public:
  NodeData() = default;
  NodeData(double lat, double lon, std::string const &name,
           std::set<Facility> const &facilities)
      : lat_(lat), lon_(lon), name_(name), facilities_(facilities) {}

private:
  double lat_;
  double lon_;
  std::string name_;
  std::set<Facility> facilities_;
};