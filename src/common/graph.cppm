export module aoc.graph;

import std;

template<typename R, typename V>
concept RangeOf =
  std::ranges::range<R> && std::same_as<std::ranges::range_value_t<R>, V>;

export namespace aoc::graph {

template<typename V, typename W>
class Graph
{
  struct Edge
  {
    std::size_t dstIndex;
    W weight;

    auto operator<(const Edge& other) const
    {
      return dstIndex < other.dstIndex;
    }
  };

  using Edges = std::set<Edge>;

protected:
  mutable std::map<std::size_t, Edges> m_Edges{};
  std::map<V, std::size_t> m_VertexMap;

private:
  class Inserter
  {
    Graph* m_Graph;
    std::size_t m_SrcIndex;
    std::size_t m_DstIndex;

    friend class Graph;

    Inserter(Graph* graph, std::size_t srcIndex, std::size_t dstIndex)
      : m_Graph(graph)
      , m_SrcIndex(srcIndex)
      , m_DstIndex(dstIndex)
    {
    }

  public:
    void operator=(const std::optional<W>& value)
    {
      if (!value) {
        m_Graph->m_Edges[m_SrcIndex].erase({ m_DstIndex, W{} });
        return;
      }
      m_Graph->m_Edges[m_SrcIndex].insert({ m_DstIndex, *value });
    }
  };

public:
  template<typename R>
    requires RangeOf<R, V>
  explicit Graph(const R& range)
  {
    std::size_t index = 0;
    for (const auto& vertex : range) {
      m_VertexMap[vertex] = index++;
    }
  }

  auto operator[](const V& dst, const V& src)
  {
    return Inserter{ this, m_VertexMap.at(src), m_VertexMap.at(dst) };
  }

  auto findAllPathsImpl(std::size_t srcIndex, std::size_t dstIndex) const
  {
    if (srcIndex == dstIndex) {
      return std::uint64_t{ 1 };
    }
    std::uint64_t count = 0;
    for (const auto& edge : m_Edges[srcIndex]) {
      count += findAllPathsImpl(edge.dstIndex, dstIndex);
    }
    return count;
  }

  auto findAllPathsCount(const V& src, const V& dst) const
  {
    const auto srcIndex = m_VertexMap.at(src);
    const auto dstIndex = m_VertexMap.at(dst);
    /**
     std::map<std::size_t, std::size_t> counts{};
     std::deque<std::size_t> toBeVisited{ srcIndex };
     counts[srcIndex] = 1;
     while (!toBeVisited.empty()) {
       const auto current = toBeVisited.back();
       toBeVisited.pop_back();
       for (const auto& edge : m_Edges[current]) {
         counts[edge.dstIndex] += counts[current];
         toBeVisited.push_back(edge.dstIndex);
       }
     }
     return counts[dstIndex];
     */

    //    if ( )
    return findAllPathsImpl(srcIndex, dstIndex);
  }
};

}