#pragma once
#include <iostream>
#include <string>

class Request {
public:
  Request() = default;
  Request(double latitude, double longitude, std::string const &medicalFacility,
          std::string const &routeType);

  double getLatitude() const;
  double getLongitude() const;
  std::string getMedicalFacility() const;
  std::string getRouteType() const;

  static Request fromJsonString(std::string &jsonString);

  friend std::ostream &operator<<(std::ostream &os, Request const &rq) {
    os << "Request: lat=" << rq.latitude << " lon=" << rq.longitude
       << " facility=" << rq.medicalFacility << " type=" << rq.routeType
       << std::endl;
    return os;
  }

private:
  double latitude;
  double longitude;
  std::string medicalFacility;
  std::string routeType;
};
