module aoc25.day11;

import std;
import aoc.graph;

namespace {

class Graph : public aoc::graph::Graph<std::string, std::uint64_t>
{
public:
  explicit Graph(const std::set<std::string>& vertices)
    : aoc::graph::Graph<std::string, std::uint64_t>(vertices)
  {
  }

private:
  std::uint64_t findAllPathsCountIncludingImpl(
    std::size_t srcIndex,
    std::size_t dstIndex,
    std::vector<std::size_t> includes) const
  {
    std::uint64_t count = 0;
    return count;
  }

public:
  auto findAllPathsCountIncluding(const std::string& src,
                                  const std::string& dst,
                                  const std::vector<std::string>& include) const
  {
    std::vector<std::size_t> includeIndexes(include.size());
    for (const auto& name : include) {
      includeIndexes.push_back(m_VertexMap.at(name));
    }
    return findAllPathsCountIncludingImpl(
      m_VertexMap.at(src), m_VertexMap.at(dst), includeIndexes);
  }
};
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
    return std::ranges::all_of(
      names, [&vertices](const auto& name) { return vertices.contains(name); });
  };

  std::uint64_t part1 = 0;
  if (verticesContain(std::array{ "you", "out" })) {
    part1 = graph.findAllPathsCount("you", "out");
  }
  std::uint64_t part2 = 0;
  if (verticesContain(std::array{ "svr", "out", "dac", "fft" })) {
    part2 = graph.findAllPathsCountIncluding("svr", "out", { "dac", "fft" });
  }
  return { part1, part2 };
}
}