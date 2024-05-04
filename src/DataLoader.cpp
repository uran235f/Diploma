#include "DataLoader.hpp"

void DataLoader::loadNodes(Graph &graph) {
  auto data =
      txn_.exec("select node.id, latitude, longitude, facility_name, "
                "address, facility_type from node inner join metadata on "
                "node.meta_id = metadata.id;");
  for (auto const &row : data) {
    std::set<Facility> facilities;
    auto arr = row["facility_type"].as_array();
    std::pair<pqxx::array_parser::juncture, std::string> elem;
    do {
      elem = arr.get_next();
      if (elem.first == pqxx::array_parser::juncture::string_value)
        facilities.insert(static_cast<Facility>(std::stoi(elem.second)));
    } while (elem.first != pqxx::array_parser::juncture::done);

    auto address = boost::trim_copy(row["address"].as<std::string>());
    auto name = boost::trim_copy(row["facility_name"].as<std::string>());

    graph.addNode(Node{row["id"].as<size_t>(), row["latitude"].as<double>(),
                       row["longitude"].as<double>(), std::move(name),
                       std::move(address), std::move(facilities)});
  }

  std::cout << "Nodes loaded." << std::endl;
}

void DataLoader::loadArcs(Graph &graph) {
  auto data = txn_.exec("select arc.id, from_node, to_node, street, distance, "
                        "transport_name, transport_type from arc inner join "
                        "transport on arc.transport_id = transport.id;");
  for (auto const &row : data) {
    std::set<TransportType> types;
    auto arr = row["transport_type"].as_array();
    std::pair<pqxx::array_parser::juncture, std::string> elem;
    do {
      elem = arr.get_next();
      if (elem.first == pqxx::array_parser::juncture::string_value)
        types.insert(static_cast<TransportType>(std::stoi(elem.second)));
    } while (elem.first != pqxx::array_parser::juncture::done);

    auto street = boost::trim_copy(row["street"].as<std::string>());
    auto name = boost::trim_copy(row["transport_name"].as<std::string>());

    graph.addArc(Arc{row["id"].as<size_t>(), row["from_node"].as<size_t>(),
                     row["to_node"].as<size_t>(), std::move(street),
                     row["distance"].as<double>(), std::move(name),
                     std::move(types)});
  }
  
    std::cout << "Arcs loaded." << std::endl;
}