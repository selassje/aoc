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

  mutable std::map<std::size_t, Edges> m_Edges{};
  std::map<V, std::size_t> m_VertexMap;

protected:
  auto& getEdges() const { return m_Edges; }
  auto& getVertexMap() const { return m_VertexMap; }

  using Memo = std::vector<std::optional<std::uint64_t>>;
  struct IndexPair
  {
    std::size_t src;
    std::size_t dst;
  };

public:
  struct VertexPair
  {
    V src;
    V dst;
  };

private:
  class Inserter
  {
    Graph* m_Graph;
    std::size_t m_SrcIndex;
    std::size_t m_DstIndex;

    friend class Graph;

    Inserter(Graph* graph, IndexPair indexPair)
      : m_Graph(graph)
      , m_SrcIndex(indexPair.src)
      , m_DstIndex(indexPair.dst)
    {
    }

  public:
    void operator=(const std::optional<W>& value) // NOLINT
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
    return Inserter{ this, { m_VertexMap.at(src), m_VertexMap.at(dst) } };
  }

  auto findAllPathsImpl(IndexPair pair, Memo& memo) const
  {
    if (pair.src == pair.dst) {
      return std::uint64_t{ 1 };
    }
    if (memo[pair.src]) {
      return *memo[pair.src];
    }
    std::uint64_t count = 0;
    for (const auto& edge : m_Edges[pair.src]) {
      count += findAllPathsImpl({ edge.dstIndex, pair.dst }, memo);
    }
    memo[pair.src] = count;
    return count;
  }

  auto findAllPathsCount(const VertexPair& pair) const
  {
    const auto srcIndex = m_VertexMap.at(pair.src);
    const auto dstIndex = m_VertexMap.at(pair.dst);
    Memo memo(m_VertexMap.size());
    return findAllPathsImpl({ srcIndex, dstIndex }, memo);
  }

  auto findShortestDistance(const VertexPair& pair) const
  {
    const auto srcIndex = m_VertexMap.at(pair.src);
    const auto dstIndex = m_VertexMap.at(pair.dst);
    static constexpr auto inf = std::numeric_limits<std::uint64_t>::max();
    std::vector<std::uint64_t> distances(m_VertexMap.size(), inf);
    distances[srcIndex] = 0;

    auto compare = [&distances](std::uint64_t lhs, std::uint64_t rhs) {
      if (distances[lhs] != distances[rhs]) {
        return distances[lhs] < distances[rhs];
      }
      return lhs < rhs;
    };

    std::set<std::uint64_t, decltype(compare)> queue(compare);
    queue.insert(srcIndex);
    while (!queue.empty()) {
      const std::uint64_t currentVertex = *queue.begin();
      queue.erase(queue.begin());
      for (const auto& edge : m_Edges[currentVertex]) {
        const std::uint64_t altDistance =
          distances[currentVertex] + edge.weight;
        if (altDistance < distances[edge.dstIndex]) {
          queue.erase(currentVertex);
          distances[edge.dstIndex] = altDistance;
          queue.insert(edge.dstIndex);
        }
      }
    }
    return distances[dstIndex];
  }
};

}