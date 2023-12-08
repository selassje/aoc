
#include "aoc23/day8.hpp"

#include <algorithm>
#include <string_view>

namespace aoc23::day8 {

Node findNodeById(const std::string_view id, const Nodes &nodes) {
    const auto it = std::ranges::find(nodes,id, &Node::id);
    return *it;
}

Result
solve(const Input& input)
{
  auto findNode = [&input](const auto& id) {
    return findNodeById(id, input.nodes);
  };

  std::size_t part1 = 0;
  std::size_t directionsIndex = 0;
  auto node = findNode("AAA");
  while(node.id != "ZZZ") {
    if (input.directions[directionsIndex] == Direction::Left) {
      node = findNode(node.left);
    } else {
      node = findNode(node.right);
    }
    directionsIndex = (directionsIndex + 1 ) % input.directions.size();
    ++part1;
  }
  return { part1, part1 };
};

}