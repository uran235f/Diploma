#pragma once

#include <iostream>
#include <string>

#include "Transport.hpp"

enum class RequestedTransport {
  PUBLIC = 1,
  CAR,
  PEDESTRIAN
};

class Request {
public:
  static Request fromJsonString(std::string &jsonString);

  double getLatitude() const { return latitude; }
  double getLongitude() const { return longitude; }
  int getMedicalFacility() const { return medicalFacility; }
  RequestedTransport getRouteType() const { return routeType; }

  friend std::ostream &operator<<(std::ostream &os, Request const &rq) {
    os << "Request: lat=" << rq.latitude << " lon=" << rq.longitude
       << " facility=" << rq.medicalFacility
       << " type=" << static_cast<int>(rq.routeType) << std::endl;
    return os;
  }

private:
  Request() = default;
  Request(double latitude, double longitude, int medicalFacility,
          RequestedTransport routeType);

  double latitude;
  double longitude;
  int medicalFacility;
  RequestedTransport routeType;
};
