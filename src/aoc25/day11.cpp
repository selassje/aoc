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
  std::uint64_t findAllPathsCountIncludingImpl(
    std::size_t srcIndex,
    std::size_t dstIndex,
    const std::vector<std::size_t>& includes,
    Memo& memo) const
  {
    if (srcIndex == dstIndex && includes.empty()) {
      return 1;
    }
    if (srcIndex == dstIndex && !includes.empty()) {
      return 0;
    }

    if (memo[srcIndex]) {
      return *memo[srcIndex];
    }

    auto newIncludes = includes;
    newIncludes.erase(std::begin(std::ranges::remove(newIncludes, srcIndex)),
                      newIncludes.end());

    std::uint64_t count = 0;
    for (const auto& edge : m_Edges[srcIndex]) {
      count += findAllPathsCountIncludingImpl(
        edge.dstIndex, dstIndex, newIncludes, memo);
    }
    memo[srcIndex] = count;
    return count;
  }
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
    for (const auto& edge : m_Edges[srcIndex]) {
      count +=
        findAllPathsCountAvodingImpl(edge.dstIndex, dstIndex, avoidIndex,memo);
    }
    memo[srcIndex] = count;
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
    Memo memo(m_VertexMap.size());
    return findAllPathsCountIncludingImpl(
      m_VertexMap.at(src), m_VertexMap.at(dst), includeIndexes, memo);
  }
  auto findAllPathsCountAvoiding(const std::string& src,
                                 const std::string& dst,
                                 const std::string& avoid) const
  {
    Memo memo(m_VertexMap.size());
    return findAllPathsCountAvodingImpl(
      m_VertexMap.at(src), m_VertexMap.at(dst), m_VertexMap.at(avoid), memo);
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

  if (graph.hasCycle()) {
    throw std::runtime_error("Graph has cycle");
  }

  // graph.print();

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
    
    std::uint64_t count2 = 0;
    
    count2 = graph.findAllPathsCountAvoiding("svr", "dac", "fft") *
            graph.findAllPathsCount("dac", "fft") *
            graph.findAllPathsCountAvoiding("fft", "out", "dac");
    count2 += graph.findAllPathsCountAvoiding("svr", "fft", "dac") *
             graph.findAllPathsCount("fft", "dac") *
             graph.findAllPathsCountAvoiding("dac", "out", "fft");
  
    part2 = count2;
  }
  return { part1, part2 };
}
}