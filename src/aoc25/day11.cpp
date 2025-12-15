module aoc25.day11;

import std;
import aoc.graph;

using BaseGraph = aoc::graph::Graph<std::string, std::uint64_t>;
using Memo = std::vector<std::optional<std::uint64_t>>;
namespace {

class Graph : public BaseGraph
{
public:
  explicit Graph(const std::set<std::string>& vertices)
    : aoc::graph::Graph<std::string, std::uint64_t>(vertices)
  {
  }

private:
  std::uint64_t findAllPathsCountAvodingImpl(std::size_t srcIndex,
                                             std::size_t dstIndex,
                                             std::size_t avoidIndex,
                                             Memo& memo) const
  {
    if (srcIndex == dstIndex) {
      return 1;
    }
    if (srcIndex == avoidIndex) {
      return 0;
    }

    if (memo[srcIndex]) {
      return *memo[srcIndex];
    }

    std::uint64_t count = 0;
    for (const auto& edge : getEdges()[srcIndex]) {
      count +=
        findAllPathsCountAvodingImpl(edge.dstIndex, dstIndex, avoidIndex, memo);
    }
    memo[srcIndex] = count;
    return count;
  }

public:
  auto findAllPathsCountAvoiding(const std::string& src,
                                 const std::string& dst,
                                 const std::string& avoid) const
  {
    const auto& vertexMap = getVertexMap();
    Memo memo(vertexMap.size());
    return findAllPathsCountAvodingImpl(
      vertexMap.at(src), vertexMap.at(dst), vertexMap.at(avoid), memo);
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
      graph[connection.input, output] = 1;
    }
  }

  auto verticesContain = [&vertices](const auto& names) {
    return std::ranges::all_of(
      names, [&vertices](const auto& name) { return vertices.contains(name); });
  };

  std::uint64_t part1 = 0;
  if (verticesContain(std::array{ "you", "out" })) {
    part1 = graph.findAllPathsCount({ "you", "out" });
  }
  std::uint64_t part2 = 0;
  if (verticesContain(std::array{ "svr", "out", "dac", "fft" })) {

    part2 = graph.findAllPathsCountAvoiding("svr", "dac", "fft") *
            graph.findAllPathsCount({ "dac", "fft" }) *
            graph.findAllPathsCountAvoiding("fft", "out", "dac");
    part2 += graph.findAllPathsCountAvoiding("svr", "fft", "dac") *
             graph.findAllPathsCount({ "fft", "dac" }) *
             graph.findAllPathsCountAvoiding("dac", "out", "fft");
  }
  return { part1, part2 };
}
}