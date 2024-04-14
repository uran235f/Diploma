#pragma once

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <pqxx/pqxx>
#include <string>

#include "Graph.hpp"

class DataLoader {
public:
  DataLoader()
      : conn_("postgresql://postgres:postgres@localhost:5432/graphdb"),
        txn_(conn_) {
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
  pqxx::connection conn_;
  pqxx::work txn_;
};