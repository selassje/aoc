export module aoc.graph;

import std;
using Memo = std::vector<std::optional<std::uint64_t>>;

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

  mutable std::map<std::size_t, Edges> m_Edges{};
  std::map<V, std::size_t> m_VertexMap;

protected:
  auto& getEdges() const { return m_Edges; }

  auto& getVertexMap() const { return m_VertexMap; }

public:
struct VertixPair{
  V src;
  V dst;
};

private:
struct IndexPair{
  std::size_t srcIndex;
  std::size_t dstIndex;
};
  class Inserter
  {
    Graph* m_Graph;
    std::size_t m_SrcIndex;
    std::size_t m_DstIndex;

    friend class Graph;

    Inserter(Graph* graph, IndexPair indexPair)
      : m_Graph(graph)
      , m_SrcIndex(indexPair.srcIndex)
      , m_DstIndex(indexPair.dstIndex)
    {
    }

  public:
    void operator=(const std::optional<W>& value) //NOLINT
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

  auto operator[](const V& src, const V& dst)
  {
    return Inserter{ this, {m_VertexMap.at(src), m_VertexMap.at(dst)} };
  }

  auto findAllPathsImpl(IndexPair pair, Memo& memo) const
  {
    if (pair.srcIndex == pair.dstIndex) {
      return std::uint64_t{ 1 };
    }
    if (memo[pair.srcIndex]) {
      return *memo[pair.srcIndex];
    }
    std::uint64_t count = 0;
   for (const auto& edge : m_Edges[pair.srcIndex]) {
      count += findAllPathsImpl({edge.dstIndex, pair.dstIndex}, memo);
    }
    memo[pair.srcIndex] = count;
    return count;
  }

public:
  auto findAllPathsCount(const VertixPair& pair) const
  {
    const auto srcIndex = m_VertexMap.at(pair.src);
    const auto dstIndex = m_VertexMap.at(pair.dst);
    Memo memo(m_VertexMap.size());
    return findAllPathsImpl({srcIndex, dstIndex}, memo);
  }
};

}