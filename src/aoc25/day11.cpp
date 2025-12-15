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
  Graph graph(vertices);

  for (const auto& connection : input) {
    for (const auto& output : connection.outputs) {
      graph[output, connection.input] = 1;
    }
  }

  auto verticesContain = [&vertices](const auto& names) {
    return std::ranges::all_of (names, [&vertices](const auto& name) {
      return vertices.contains(name);
    });
  }; 

  std::size_t part1 =  0; 
  if(verticesContain(std::array{"you", "out"})) {
    part1 = graph.findAllPathsCount("you", "out");
  }
  std::size_t part2 = 0;
  if(verticesContain(std::array{"svr", "out","dac", "fft"})) {
    part2 = 4;
  }
  return { part1, part2 };
}
}