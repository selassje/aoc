module;

#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <vector>

export module aoc.matrix;

export namespace aoc::matrix {

template<typename T>
class Matrix
{
private:
  const std::size_t m_Rows;
  const std::size_t m_Cols;
  std::vector<T> m_Data;

  using Vector = std::vector<std::vector<T>>;

public:
  Matrix(Vector vec)
    : m_Rows(vec.size())
    , m_Cols(vec.empty() ? 0 : vec[0].size())
  {
    if (m_Rows == 0 || m_Cols == 0) {
      throw std::runtime_error("Matrix dimensions cannot be zero");
    }
    m_Data.reserve(m_Rows * m_Cols);
    for (const auto& row : vec) {
      if (row.size() != m_Cols) {
        throw std::runtime_error(
          "Inconsistent row sizes in matrix initialization");
      }
      std::ranges::copy(row, std::back_inserter(m_Data));
    }
  }

  const T& operator[](std::size_t x, std::size_t y) const
  {
    if (x >= m_Cols || y >= m_Rows) {
      throw std::out_of_range("Matrix index out of range");
    }
    return m_Data[(y * m_Cols) + x];
  }

  T& operator[](std::size_t x, std::size_t y)
  {
    if (x >= m_Cols || y >= m_Rows) {
      throw std::out_of_range("Matrix index out of range");
    }
    return m_Data[(y * m_Cols) + x];
  }

  [[nodiscard]] std::size_t getHeight() const { return m_Rows; }
  [[nodiscard]] std::size_t getWidth() const { return m_Cols; }
};

}