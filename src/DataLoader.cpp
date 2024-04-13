#include "DataLoader.hpp"

void DataLoader::loadNodes(Graph &graph) {
  pqxx::connection conn(
      "postgresql://postgres:postgres@localhost:5432/graphdb");
  pqxx::work txn{conn};

  auto data =
      txn.exec("select node.id, latitude, longitude, facility_name, "
               "facility_type, address from node inner join metadata on "
               "node.meta_id = metadata.id;");
  for (auto row : data) {
    std::set<Facility> facilities;
    auto arr = row["facility_type"].as_array();
    std::pair<pqxx::array_parser::juncture, std::string> elem;
    do {
      elem = arr.get_next();
      if (elem.first == pqxx::array_parser::juncture::string_value)
        facilities.insert(static_cast<Facility>(std::stoi(elem.second)));
    } while (elem.first != pqxx::array_parser::juncture::done);

    graph.addNode(Node{
        row["id"].as<size_t>(), row["latitude"].as<double>(),
        row["longitude"].as<double>(), row["facility_name"].as<std::string>(),
        std::move(facilities), row["address"].as<std::string>()});
  }
}

void DataLoader::loadArcs(Graph &graph) {
  pqxx::connection conn(
      "postgresql://postgres:postgres@localhost:5432/graphdb");
  pqxx::work txn{conn};

  auto data =
      txn.exec("select node.id, latitude, longitude, facility_name, "
               "facility_type, address from node inner join metadata on "
               "node.meta_id = metadata.id;");
  for (auto row : data) {
    std::set<Facility> facilities;
    auto arr = row["facility_type"].as_array();
    std::pair<pqxx::array_parser::juncture, std::string> elem;
    do {
      elem = arr.get_next();
      if (elem.first == pqxx::array_parser::juncture::string_value)
        facilities.insert(static_cast<Facility>(std::stoi(elem.second)));
    } while (elem.first != pqxx::array_parser::juncture::done);

    graph.addNode(Node{
        row["id"].as<size_t>(), row["latitude"].as<double>(),
        row["longitude"].as<double>(), row["facility_name"].as<std::string>(),
        std::move(facilities), row["address"].as<std::string>()});
  }
}

// void DataLoader::loadArcs(Graph &graph) {
//   for (auto [id] : txn.exec("SELECT id FROM Arc")) {
//     continue;
//   }
// }