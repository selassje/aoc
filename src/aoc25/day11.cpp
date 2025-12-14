module aoc25.day11;

import std;
import aoc.graph;

using Graph = aoc::graph::Graph<std::string, std::uint64_t>;

namespace {


}

namespace aoc25::day11 {

Result
solve(const Input& input)
{
  std::set<std::string> vertices;
  for (const auto& connection : input) {
    vertices.insert(connection.input);
    vertices.insert(connection.outputs.begin(), connection.outputs.end());
  }
  vertices.insert("you");
  Graph graph(vertices);

  for(const auto& connection : input) {
    for (const auto& output : connection.outputs) {
      graph[connection.input, output] = 1;
      graph[output, connection.input] = 1;
    }
  }

  std::size_t part1 = graph.findAllPathsCount("you", "out");
  std::size_t part2 = part1;

  return {part1, part2};
}
}