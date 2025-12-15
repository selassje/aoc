module aoc25.day11;

import std;
import aoc.graph;

using BaseGraph = aoc::graph::Graph<std::string, std::uint64_t>;
namespace {

class Graph : public BaseGraph
{
  using Memo = BaseGraph::Memo;
  using IndexPair = BaseGraph::IndexPair;
  using VertixPair = BaseGraph::VertexPair;

public:
  explicit Graph(const std::set<std::string>& vertices)
    : aoc::graph::Graph<std::string, std::uint64_t>(vertices)
  {
  }

private:
  std::uint64_t findAllPathsCountAvodingImpl(IndexPair pair,
                                             std::size_t avoidIndex,
                                             Memo& memo) const
  {
    if (pair.src == pair.dst) {
      return 1;
    }
    if (pair.src == avoidIndex) {
      return 0;
    }

    if (memo[pair.src]) {
      return *memo[pair.src]; // NOLINT
    }

    std::uint64_t count = 0;
    for (const auto& edge : getEdges()[pair.src]) {
      count += findAllPathsCountAvodingImpl(
        { edge.dstIndex, pair.dst }, avoidIndex, memo);
    }
    memo[pair.src] = count;
    return count;
  }

public:
  auto findAllPathsCountAvoiding(const VertixPair& pair,
                                 const std::string& avoid) const
  {
    const auto& vertexMap = getVertexMap();
    Memo memo(vertexMap.size());
    return findAllPathsCountAvodingImpl(
      { vertexMap.at(pair.src), vertexMap.at(pair.dst) },
      vertexMap.at(avoid),
      memo);
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

    part2 = graph.findAllPathsCountAvoiding({ "svr", "dac" }, "fft") *
            graph.findAllPathsCount({ "dac", "fft" }) *
            graph.findAllPathsCountAvoiding({ "fft", "out" }, "dac");
    part2 += graph.findAllPathsCountAvoiding({ "svr", "fft" }, "dac") *
             graph.findAllPathsCount({ "fft", "dac" }) *
             graph.findAllPathsCountAvoiding({ "dac", "out" }, "fft");
  }
  return { part1, part2 };
}
}