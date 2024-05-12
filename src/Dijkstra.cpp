#include "Dijkstra.hpp"
#include "Heap.hpp"

#include <algorithm>
#include <map>

using json = nlohmann::json;

std::string Dijkstra::optimize(Graph const &g, std::size_t from,
                               std::size_t to) {
  Heap<Item> queue;
  std::map<size_t, Item> visited;

  std::size_t destination_node = to;
  std::size_t start_node = from;
  auto start = Item{start_node, std::nullopt, std::nullopt, nullptr, 0};

  bool reached_dest = false;
  std::optional<Item> result;

  queue.push(start);

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

      if (arc.to() == destination_node) {
        result =
            Item{arc.to(), arc_id, *arc.transportData()->transport().begin(),
                 std::make_shared<Item>(item), arc.distance() + item.distance};
        reached_dest = true;
        break;
      }

      auto it = visited.find(arc.to());
      if (it == visited.end()) {
        Item new_item =
            Item{arc.to(), arc_id, *arc.transportData()->transport().begin(),
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

        std::cout << "[VISITED][CURR] " << item << std::endl;

        if (new_dist < it->second.distance) {

          auto to_update = queue.find(it->second);
          to_update->distance = new_dist;
          to_update->parent = item.parent;
          to_update->parent_arc = arc_id;
          to_update->transport = *arc.transportData()->transport().begin();

          it->second.distance = new_dist;
          it->second.parent = item.parent;
          it->second.parent_arc = arc_id;
          it->second.transport = *arc.transportData()->transport().begin();

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

    if (reached_dest) {
      break;
    }
  }

  std::string json_result = "";

  if (result) {
    std::cout << "Shortest dist=" << result->distance << std::endl;
    Item::print_chain(*result, g);
    return generate_json_result(*result, g, start_node);
  }

  std::cout << "No path to dest=" << destination_node << std::endl;
  return "";
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
      add_arc_json(node_json, it->transport.value(),
                   g.arc(*it->parent_arc).transportData()->name());
      result.push_back(node_json);
    }
  }

  make_node_json(g.node(start));
  std::reverse(result.begin(), result.end());

  // if (result.size() > 25) {
  //   auto new_result = reduce_result(result);
  //   add_json_distance(new_result, item.distance);
  //   return to_string(new_result);
  // }

  add_additional_fields(result, item.distance);
  return to_string(result);
}

void Dijkstra::add_additional_fields(json &to, double dist) {
  json distance;
  distance["distance"] = std::to_string(dist);
  to.push_back(distance);
}

json Dijkstra::reduce_result(json &modifiable) {
  std::cout << "modifiable size=" << modifiable.size() << std::endl;

  std::size_t index = (modifiable.size() - 2) / 23;
  std::cout << "index=" << index << std::endl;

  char letter = 'A';

  json rv;
  rv.push_back(modifiable.front());
  rv.back()["ASTANA"] = static_cast<char>(letter);
  ++letter;

  auto it = modifiable.begin() + index;
  for (auto i = 0; i != 23; ++i, it += index) {
    rv.push_back(*it);
    rv.back()["ASTANA"] = static_cast<char>(letter);
    ++letter;
  }
  rv.push_back(modifiable.back());
  rv.back()["ASTANA"] = static_cast<char>(letter);

  return rv;
}

void Item::print_chain(Item const &item, Graph const &g) {
  for (Item const *it = &item; it != nullptr; it = it->parent.get()) {
    if (it->parent_arc) {
      auto arc = g.arc(*it->parent_arc);
      std::cout << "[DEBUG] from [" << arc.from() << "] -> "
                << "via {" << arc.arc() << "} -> "
                << "to [" << arc.to() << "] with distance " << it->distance
                << std::endl;
    }
  }
}