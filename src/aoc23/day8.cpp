
#include "aoc23/day8.hpp"

#include <algorithm>
#include <iostream>
#include <optional>
#include <ranges>
#include <string_view>

namespace aoc23::day8 {

std::optional<Node>
findNodeById(const std::string_view id, const Nodes& nodes)
{
  std::optional<Node> node = std::nullopt;
  const auto it = std::ranges::find(nodes, id, &Node::id);
  if (it != nodes.end()) {
    node = *it;
  }
  return node;
}
Nodes
findNodesEndingIn(const Nodes& nodes, const char end)
{
  auto filtered = nodes | std::views::filter([&end](const auto& node) {
                    return node.id[2] == end;
                  });
  return Nodes(filtered.begin(), filtered.end());
}

Result
solve(const Input& input)
{
  auto findNode = [&input](const auto& id) {
    return findNodeById(id, input.nodes);
  };

  std::size_t part1 = 0;
  std::size_t directionsIndex = 0;
  auto maybeNode = findNode("AAA");
  if (maybeNode) {
    auto node = *maybeNode;
    while (node.id != "ZZZ") {
      if (input.directions[directionsIndex] == Direction::Left) {
        node = *findNode(node.left);
      } else {
        node = *findNode(node.right);
      }
      directionsIndex = (directionsIndex + 1) % input.directions.size();
      ++part1;
    }
  }

  const auto startNodes = findNodesEndingIn(input.nodes, 'A');
  const auto startNodesCount = startNodes.size();

  std::cout << startNodesCount << std::endl;

  return { part1, part1 };
};

}