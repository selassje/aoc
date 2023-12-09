
#include "aoc23/day8.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <optional>
#include <ranges>
#include <string_view>

namespace aoc23::day8 {

template<typename F>
std::size_t
stepsTo(const Node& startNode, const Input& input, F cond)
{
  std::size_t result = 0;
  std::size_t directionsIndex = 0;

  auto findNode = [&input](const std::string_view id) {
    return *std::ranges::find(input.nodes, id, &Node::id);
  };

  auto node = startNode;
  while (!cond(node.id)) {
    if (input.directions[directionsIndex] == Direction::Left) {
      node = findNode(node.left);
    } else {
      node = findNode(node.right);
    }
    directionsIndex = (directionsIndex + 1) % input.directions.size();
    ++result;
  }
  return result;
}

Result
solve(const Input& input)
{
  std::size_t part1 = 0;
  std::size_t part2 = 1;
  for (const auto& node : input.nodes) {
    if (node.id == "AAA") {
      part1 = stepsTo(
        node, input, [](const std::string_view id) { return id == "ZZZ"; });
    }
    if (node.id[2] == 'A') {
      part2 =
        std::lcm(part2, stepsTo(node, input, [](const std::string_view id) {
                   return id[2] == 'Z';
                 }));
    }
  }
  return { part1, part2 };
};

}