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

  DataLoader(DataLoader const &) = delete;
  DataLoader(DataLoader &&) = delete;
  DataLoader &operator=(DataLoader const &) = delete;
  DataLoader &operator=(DataLoader &&) = delete;

  void loadNodes(Graph &graph);
  void loadArcs(Graph &graph);
  static void load(Graph &graph);

private:
  pqxx::connection conn_;
  pqxx::work txn_;
};