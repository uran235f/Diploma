#include "Dijkstra.hpp"
#include "Heap.hpp"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <map>
#include <queue>
#include <ranges>

using json = nlohmann::json;

std::string Dijkstra::optimize(Graph const &g, Request const &request) {

  auto closest_departure_node = Dijkstra::find_closed_node(
      g, request.getLatitude(), request.getLongitude());

  if (!closest_departure_node) {
    json status;
    status["status"] = "Closest node not found. Graph is empty";
    return to_string(status);
  }

  Heap<Item> queue;
  std::map<size_t, Item> visited;

  std::size_t depature_node = *closest_departure_node;

  auto destination_node_opt =
      bfs(g, depature_node, request.getMedicalFacility());
  if (destination_node_opt) {
    std::cout << "Can reach facility type " << request.getMedicalFacility()
              << " at node " << *destination_node_opt << std::endl;
  } else {
    std::cout << "[WARN] Facility type " << request.getMedicalFacility()
              << " is unreachable from node " << depature_node << std::endl;
    return "";
  }

  std::size_t destination_node = *destination_node_opt;
  auto start = Item{depature_node, std::nullopt, TransportType::pedestrian,
                    nullptr, 0.0};

  std::optional<Item> result;
  queue.push(start);

  auto speed_comparator = [&](auto lhs, auto rhs) {
    return Item::transportToSpeed(lhs) < Item::transportToSpeed(rhs);
  };

  std::cout << "Dijkstra optimization configured." << std::endl;

  while (!queue.empty()) {
    auto item = queue.top();
    queue.pop();

    std::cout << "[WORKING] node_id=" << item.node_id << std::endl;
    auto arcs = g[item.node_id];

    if (!arcs.has_value()) {
      std::cout << "[WORKING] no arcs available" << std::endl;
      continue;
    }

    for (auto arc_id : arcs.value()) {
      auto const &arc = g.arc(arc_id);
      auto const &transport = g.arc(arc_id).transportData()->transport();
      auto requestedType = request.getRouteType();
      auto selected = select_transport(transport, requestedType);

      print_transport("All transports: ", transport);
      print_transport("Selected transport: ", selected);

      auto fastest_transport =
          std::ranges::max_element(selected, speed_comparator);
      auto highest_speed = Item::transportToSpeed(*fastest_transport);

      std::cout << "fasters transport=" << static_cast<int>(*fastest_transport)
                << " speed=" << highest_speed << std::endl;

      if (arc.to() == destination_node) {
        result =
            Item{arc.to(), arc_id, *fastest_transport,
                 std::make_shared<Item>(item), arc.distance() + item.distance};
        goto found_solution;
      }

      auto it = visited.find(arc.to());
      if (it == visited.end()) {
        Item new_item =
            Item{arc.to(), arc_id, *fastest_transport,
                 std::make_shared<Item>(item), arc.distance() + item.distance};

        std::cout << "[CURR] " << item << std::endl;
        std::cout << "[NEW ITEM] " << new_item << std::endl;

        std::cout << "Not visited: to_id=" << arc.to() << " via arc=" << arc_id
                  << " cost=" << new_item.distance << std::endl
                  << std::endl;

        visited[arc.to()] = new_item;
        queue.push(new_item);
      } else {

        auto new_dist = arc.distance() + item.distance;
        auto new_time = new_dist * 3600 / (highest_speed * 1000);

        std::cout << "[VISITED][CURR] " << item << std::endl;

        if (new_time < it->second.time) {

          auto to_update = queue.find(it->second);
          to_update->distance = new_dist;
          to_update->parent = item.parent;
          to_update->parent_arc = arc_id;
          to_update->transport = *fastest_transport;
          to_update->time = new_time;

          it->second.distance = new_dist;
          it->second.time = new_time;
          it->second.parent = item.parent;
          it->second.parent_arc = arc_id;
          it->second.transport = *fastest_transport;

          std::cout << "[UPDATE] to_id=" << arc.to() << " via arc=" << arc_id
                    << " cost=" << it->second.distance << std::endl;
          std::cout << "[ITEM] " << item << std::endl;

          queue.update();

        } else {
          std::cout << "[EXPENSIVE] to_id=" << arc.to() << " via arc=" << arc_id
                    << " cost=" << it->second.distance << std::endl;
        }
      }
    }
  }

found_solution:

  std::string json_result = "";

  if (result) {
    std::cout << "Shortest dist=" << result->distance << std::endl;
    Item::print_chain(*result, g);
    return generate_json_result(*result, g, depature_node);
  }

  //   std::stringstream oss;
  // oss << "No path to dest=" << destination_node << std::endl;
  // json json_result;
  // json_result["status"] = oss.str();
  // return to_string(json_result);

  std::cout << "No path to dest=" << destination_node << std::endl;
  return "";
}

template <typename Containter>
void Dijkstra::print_transport(std::string const &prepend,
                               Containter const &container) {
  std::cout << prepend;
  for (auto type : container) {
    std::cout << static_cast<int>(type) << " ";
  }
  std::cout << std::endl;
}

std::optional<nodeId> Dijkstra::bfs(Graph const &g,
                                    std::optional<nodeId> const &from,
                                    int requested_facility) {
  if (!from) {
    return std::nullopt;
  }

  std::set<nodeId> visited;
  std::optional<nodeId> destination;
  std::queue<nodeId> queue;
  queue.push(*from);

  while (!destination.has_value() && !queue.empty()) {
    auto node_id = queue.front();
    queue.pop();

    if (visited.count(node_id)) {
      continue;
    }

    auto outgoing_arcs = g[node_id];
    std::cout << "outgoing_arcs size=" << outgoing_arcs->size() << std::endl;
    if (!outgoing_arcs.has_value()) {
      continue;
    }

    for (auto const &arc_id : outgoing_arcs.value()) {
      auto to_node = g.arc(arc_id).to();

      auto const &node = g.node(to_node);
      auto const &facilities = node.data()->facilities();
      std::cout << "facilities size=" << facilities.size() << std::endl;
      for (auto f : facilities) {
        std::cout << "f=" << static_cast<int>(f) << " ";
      }
      std::cout << std::endl;
      auto found =
          std::any_of(facilities.begin(), facilities.end(), [=](auto facility) {
            return facility == static_cast<Facility>(requested_facility);
          });

      std::cout << "found=" << (int)found << std::endl;

      if (found) {
        destination = to_node;
        break;
      }
      visited.insert(node_id);
      queue.push(to_node);
    }
  }
  return destination;
}

std::vector<TransportType>
Dijkstra::select_transport(std::set<TransportType> const &transport,
                           RequestedTransport requestedType) {
  std::vector<TransportType> selected;

  std::ranges::copy_if(transport, std::back_inserter(selected), [&](auto type) {
    return ((requestedType == RequestedTransport::PUBLIC) &&
            (type == TransportType::subway || type == TransportType::bus)) ||
           (requestedType == RequestedTransport::CAR &&
            type == TransportType::car) ||
           (type == TransportType::pedestrian);
  });
  return selected;
}

std::optional<nodeId> Dijkstra::find_closed_node(Graph const &g, double lat,
                                                 double lon) {
  auto min_distance = std::numeric_limits<double>::max();
  std::optional<nodeId> closest = std::nullopt;
  for (auto const &[node_id, node] : g.nodes()) {
    auto lat_diff = abs(lat - node.data()->lat());
    auto lon_diff = abs(lon - node.data()->lon());
    auto distance = sqrt((pow(lat_diff, 2) + pow(lon_diff, 2)));
    if (min_distance > distance) {
      min_distance = distance;
      closest = node_id;
    }
  }
  return closest;
}

std::string Dijkstra::generate_json_result(Item const &item, Graph const &g,
                                           std::size_t start) {
  json result;

  auto make_node_json = [&](Node const &node) -> nlohmann::json {
    json json_item;
    json_item["node"] = std::to_string(node.node());
    json_item["lat"] = std::to_string(node.data()->lat());
    json_item["lon"] = std::to_string(node.data()->lon());
    json_item["name"] = node.data()->name();
    json_item["facilities"] = node.data()->facilities();
    return json_item;
  };

  auto add_arc_json = [](json &json, TransportType transport,
                         std::string const &name) -> nlohmann::json {
    json["transport_name"] = name;
    json["transport"] = transport;
    return json;
  };

  for (Item const *it = &item; it != nullptr; it = it->parent.get()) {
    if (it->parent_arc) {
      auto node_json = make_node_json(g.node(it->node_id));
      add_arc_json(node_json, it->transport,
                   g.arc(*it->parent_arc).transportData()->name());
      result.push_back(node_json);
    }
  }

  make_node_json(g.node(start));
  std::reverse(result.begin(), result.end());

  add_additional_fields(result, item.distance);
  return to_string(result);
}

void Dijkstra::add_additional_fields(json &to, double dist) {
  json distance;
  distance["distance"] = std::to_string(dist);
  to.push_back(distance);
}

void Item::print_chain(Item const &item, Graph const &g) {
  for (Item const *it = &item; it != nullptr; it = it->parent.get()) {
    if (it->parent_arc) {
      auto arc = g.arc(*it->parent_arc);
      std::cout << "[DEBUG] from [" << arc.from() << "] -> "
                << "via {" << arc.arc() << "} -> "
                << "to [" << arc.to() << "] with distance " << it->distance
                << " time " << it->time << std::endl;
    }
  }
}

double Item::transportToSpeed(TransportType transport) {
  switch (transport) {
  case TransportType::subway:
    return static_cast<double>(Item::Speeds::SUBWAY);
  case TransportType::bus:
    return static_cast<double>(Item::Speeds::BUS);
  case TransportType::car:
    return static_cast<double>(Item::Speeds::CAR);
  case TransportType::pedestrian:
    return static_cast<double>(Item::Speeds::PERDESTRIAN);
  default:
    break;
    throw "Unknown transport type";
  }
}