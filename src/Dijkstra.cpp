#include "Dijkstra.hpp"
#include <map>
#include <nlohmann/json.hpp>
#include <queue>

std::string Dijkstra::optimize(Graph const &g, std::size_t from,
                               std::size_t to) {
  std::priority_queue<Item, std::vector<Item>, std::greater<Item>> queue;
  std::map<size_t, Item> visited;

  std::size_t destination_node = to;
  std::size_t start_node = from;
  auto start = Item{start_node, std::nullopt, nullptr, 0};

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
      continue;
    }
    
    for (auto arc_id : arcs.value()) {
      auto const &arc = g.arc(arc_id);

      if (arc.to() == destination_node) {
        result = Item{arc.to(), arc_id, std::make_shared<Item>(item),
                      arc.distance() + item.distance};
        reached_dest = true;
        break;
      }

      auto it = visited.find(arc.to());
      if (it == visited.end()) {
        Item new_item = Item{arc.to(), arc_id, std::make_shared<Item>(item),
                             arc.distance() + item.distance};

        std::cout << "[ITEM] " << item << std::endl;
        std::cout << "[ITEM] " << new_item << std::endl;

        std::cout << "Not visited: to_id=" << arc.to() << " via arc=" << arc_id
                  << " cost=" << new_item.distance << std::endl;

        visited[arc.to()] = new_item;
        queue.push(new_item);
      } else {
        auto new_dist = arc.distance() + item.distance;

        if (new_dist == it->second.distance) {
          // Item new_item =
          //     Item{arc.to(), arc_id, std::make_shared<Item>(item), new_dist};
          // std::cout << "[COLLOCATION] to_id=" << arc.to()
          //           << " via arc=" << arc_id << " cost=" << new_item.distance
          //           << std::endl;
          // std::cout << "with: to_id=" << it->second.node_id
          //           << " via arc=" << *it->second.parent_arc
          //           << " cost=" << it->second.distance << std::endl;

          // std::cout << "[ITEM] " << item << std::endl;
          // std::cout << "[ITEM] " << new_item << std::endl;

          // queue.push(new_item);
        } else if (new_dist < it->second.distance) {
          it->second.distance = new_dist;
          it->second.parent = std::make_shared<Item>(item);
          it->second.parent_arc = arc_id;
          std::cout << "[UPDATE] to_id=" << arc.to() << " via arc=" << arc_id
                    << " cost=" << it->second.distance << std::endl;

          std::cout << "[ITEM] " << item << std::endl;

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

std::string Dijkstra::generate_json_result(Item const &item, Graph const &g,
                                           std::size_t start) {
  using json = nlohmann::json;
  json result;

  auto make_json_from = [&result](Node const &node) {
    json json_item;
    json_item["node"] = std::to_string(node.node());
    json_item["lat"] = std::to_string(node.data()->lat());
    json_item["lon"] = std::to_string(node.data()->lon());
    json_item["name"] = node.data()->name();
    json_item["facilities"] = node.data()->facilities();
    result.push_back(json_item);
  };

  for (Item const *it = &item; it != nullptr; it = it->parent.get()) {
    if (it->parent_arc) {
      make_json_from(g.node(it->node_id));
    }
  }
  make_json_from(g.node(start));
  std::reverse(result.begin(), result.end());

  json distance;
  distance["distance"] = std::to_string(item.distance);
  result.push_back(distance);

  return nlohmann::to_string(result);
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