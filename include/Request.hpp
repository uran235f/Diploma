#pragma once

#include <iostream>
#include <string>

class Request {
public:
  static Request fromJsonString(std::string &jsonString);

  double getLatitude() const { return latitude; }
  double getLongitude() const { return longitude; }
  int getMedicalFacility() const { return medicalFacility; }
  int getRouteType() const { return routeType; }

  friend std::ostream &operator<<(std::ostream &os, Request const &rq) {
    os << "Request: lat=" << rq.latitude << " lon=" << rq.longitude
       << " facility=" << rq.medicalFacility << " type=" << rq.routeType
       << std::endl;
    return os;
  }

private:
  Request() = default;
  Request(double latitude, double longitude, int medicalFacility,
          int routeType);

  double latitude;
  double longitude;
  int medicalFacility;
  int routeType;
};
