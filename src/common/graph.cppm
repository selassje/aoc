module;
#include <cassert>
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
      if (!m_Graph->m_Edges.contains(m_DstIndex)) {
        m_Graph->m_Edges[m_DstIndex] = {};
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

private:
  auto findAllPathsImpl(std::size_t srcIndex, std::size_t dstIndex, Memo& memo) const
  {
    if (srcIndex == dstIndex) {
    //  std::println("Found dst {}",dstIndex);
      return std::uint64_t{ 1 };
    }
    if (memo[srcIndex]) {
      return *memo[srcIndex];
    }

    std::uint64_t count = 0;
    for (const auto& edge : m_Edges[srcIndex]) {
      count += findAllPathsImpl(edge.dstIndex, dstIndex,memo);
    }
    memo[srcIndex]  = count;
    return count;
  }

bool hasCycleImpl(std::size_t v, std::vector<std::uint8_t>& color) const
{
    color[v] = 1; // GRAY

    auto it = m_Edges.find(v);
    if (it != m_Edges.end()) {
        for (const auto& edge : it->second) {
            std::size_t dst = edge.dstIndex;
            if (color[dst] == 1) {
                std::println("Found cycle at {}",dst);
                return true; // back edge
            }
            if (color[dst] == 0 && hasCycleImpl(dst, color))
                return true;
        }
    }

    color[v] = 2; // BLACK
    return false;
}

public:
  bool hasCycle() const
{
    std::vector<std::uint8_t> color(m_VertexMap.size(), 0); // 0 = WHITE

    for (const auto& [vertex, index] : m_VertexMap) {
        if (color[index] == 0 && hasCycleImpl(index, color))
            return true;
    }

    return false;
}


  auto findAllPathsCount(const V& src, const V& dst) const
  {
    const auto srcIndex = m_VertexMap.at(src);
    const auto dstIndex = m_VertexMap.at(dst);
    Memo memo(m_VertexMap.size());
    return findAllPathsImpl(srcIndex, dstIndex, memo);
  }
  void print()
  {
    for (const auto& [vertex, index] : m_VertexMap) {
      std::cout << vertex << " -> ";
      for (const auto& edge : m_Edges.at(index)) {
        std::cout << edge.dstIndex << " ";
      }
      std::cout << "\n";
    }
  }
};

}