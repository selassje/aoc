
#include "aoc23/day8.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
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

constexpr auto lcm(auto x, auto... xs)
{
    return ((x = std::lcm(x, xs)), ...);
}

std::size_t 
stepsToNodeEndingWith(const Node& startNode, const Input& input)
{
  std::size_t result = 0;
  std::size_t directionsIndex = 0;
  auto node = startNode;
  while (node.id[2] != 'Z') {
    if (input.directions[directionsIndex] == Direction::Left) {
      node = *findNodeById(node.left, input.nodes);
    } else {
      node = *findNodeById(node.right, input.nodes);
    }
    directionsIndex = (directionsIndex + 1) % input.directions.size();
    ++result;
  }
  std::cout << "Start " << startNode.id << " End " << node.id << "  Steps" << result <<std::endl;
  return result;
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
  std::size_t part2 = 1;

  for ( const auto& startNode : startNodes) {
    part2 = lcm(stepsToNodeEndingWith(startNode,input),part2);
  }

  std::cout << startNodesCount << std::endl;

  return { part1, part2 };
};

}