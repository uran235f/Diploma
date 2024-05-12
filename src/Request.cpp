#include "Request.hpp"
#include "nlohmann/json.hpp"

Request::Request(double latitude, double longitude,
                 std::string const &medicalFacility,
                 std::string const &routeType)
    : latitude(latitude), longitude(longitude),
      medicalFacility(medicalFacility), routeType(routeType) {}

double Request::getLatitude() const { return latitude; }
double Request::getLongitude() const { return longitude; }

std::string Request::getMedicalFacility() const { return medicalFacility; }
std::string Request::getRouteType() const { return routeType; }

Request Request::fromJsonString(std::string &jsonString) {
  std::cout << "request : " << jsonString << std::endl;
  auto json = nlohmann::json::parse(jsonString);
  return {json["latitude"].get<double>(), json["longitude"].get<double>(),
          json["medicalFacility"].get<std::string>(),
          json["routeType"].get<std::string>()};
}