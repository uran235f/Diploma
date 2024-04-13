#pragma once

#include <iostream>
#include <pqxx/pqxx>
#include <string>

#include "Graph.hpp"

class DataLoader {
public:
  DataLoader() {
    std::cout << "DataLoader initialized" << std::endl;
  }
//   DataLoader(const std::string &dbname, const std::string &user,
//              const std::string &password, const std::string &host,
//              const std::string &port)
//       : conn("jdbc:postgresql://localhost:5432/graphdb"), txn(conn) {
//     std::cout << "DataLoader initialized" << std::endl;
//   }

  void loadNodes(Graph &graph);
  void loadArcs(Graph &graph);

private:
//   pqxx::connection conn;
//   pqxx::work txn;
};