#include "Dijkstra.hpp"
#include <map>
#include <queue>

std::string Dijkstra::optimize(Graph const &g) {
  std::priority_queue<Item, std::vector<Item>, std::greater<Item>> queue;
  std::map<size_t, Item> visited;

  auto start = Item{0, std::nullopt, nullptr, 0};
  auto destination_node = 8;
  bool reached_dest = false;
  std::optional<Item> result;

  queue.push(start);

  for (auto const &[arc_id, arc] : g.arcs()) {
    std::cout << "[DEBUG] arc=" << arc_id << " arc_dist=" << arc.distance()
              << std::endl;
  }

  for (auto const &[node_id, node] : g.nodes()) {
    std::cout << "[DEBUG] arc=" << node_id << std::endl;
  }

  std::cout << "Dijkstra optimization configured." << std::endl;

  while (!queue.empty()) {
    auto item = queue.top();
    queue.pop();

    std::cout << "[WORKING] node_id=" << item.node_id << std::endl;
    auto arcs = g[item.node_id];

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

        std::cout << "prev dist=" << item.distance
                  << " arc_dist=" << arc.distance() << std::endl;

        std::cout << "Not visited: to_id=" << arc.to() << " via arc=" << arc_id
                  << " cost=" << new_item.distance << std::endl;

        visited[arc.to()] = new_item;
        queue.push(new_item);
      } else {
        auto new_dist = arc.distance() + item.distance;

        if (new_dist == it->second.distance) {
          Item new_item =
              Item{arc.to(), arc_id, std::make_shared<Item>(item), new_dist};
          std::cout << "[COLLOCATION] to_id=" << arc.to()
                    << " via arc=" << arc_id << " cost=" << new_item.distance
                    << std::endl;
          std::cout << "with: to_id=" << it->second.node_id
                    << " via arc=" << *it->second.parent_arc
                    << " cost=" << it->second.distance << std::endl;

          queue.push(new_item);
        } else if (new_dist < it->second.distance) {
          it->second.distance = new_dist;
          it->second.parent = std::make_shared<Item>(item);
          it->second.parent_arc = arc_id;
          std::cout << "[UPDATE] to_id=" << arc.to() << " via arc=" << arc_id
                    << " cost=" << it->second.distance << std::endl;
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

  if (result) {
    std::cout << "Shortest dist=" << result->distance << std::endl;
  } else {
    std::cout << "No path to dest=" << destination_node << std::endl;
  }

  return "";
}