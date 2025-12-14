export module aoc.graph;

import aoc.matrix;
import std;

using aoc::matrix::Matrix;

template<typename R, typename V>
concept RangeOf =
  std::ranges::range<R> && std::same_as<std::ranges::range_value_t<R>, V>;

export namespace aoc::graph {

template<typename V, typename E>

class Graph
{
  Matrix<std::optional<E>> m_Edges{};
  std::map<V, std::size_t> m_VertexMap;

public:
  template<typename R>
    requires RangeOf<R, V>
  explicit Graph(const R& range)
  {
    std::size_t index = 0;
    for (const auto& vertex : range) {
      m_VertexMap[vertex] = index++;
    }
    m_Edges = Matrix<std::optional<E>>{ { index, index }, {} };
  }

  auto& operator[](const V& dst, const V& src)
  {
    return m_Edges[m_VertexMap.at(dst),m_VertexMap.at(src)];
  }

  auto findAllPathsCount(const V& src, const V& dst) const
  {
    std::size_t  count = 0;
    const auto srcIndex = m_VertexMap.at(src);       
    const auto dstIndex = m_VertexMap.at(dst);
    std::vector<std::size_t> toBeVisited{ srcIndex };
    return count;
  }

};

}