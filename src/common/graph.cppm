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

  std::map<std::size_t, Edges> m_Edges{};
  std::map<V, std::size_t> m_VertexMap;

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
        m_Graph->m_Edges[m_SrcIndex].erase({m_DstIndex,W{}});
        return;
      }
      m_Graph->m_Edges[m_SrcIndex].insert({m_DstIndex, *value });
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

  auto findAllPathsCount(const V& src, const V& dst) const
  {
    std::size_t count = 0;
    const auto srcIndex = m_VertexMap.at(src);
    const auto dstIndex = m_VertexMap.at(dst);
    std::vector<std::size_t> toBeVisited{ srcIndex };
    return count;
  }
};

}