module;

#include <algorithm>
#include <cstdlib>
#include <print>
#include <ranges>
#include <stdexcept>
#include <vector>

export module aoc.matrix;

export namespace aoc::matrix {

struct Dimension
{
  std::size_t cols;
  std::size_t rows;
};

template<typename T>
class Matrix
{
private:
  std::size_t m_Cols;
  std::size_t m_Rows;
  std::vector<T> m_Data;

  template<typename R>
  using Vector = std::vector<std::vector<R>>;

public:
  template<typename R, typename Proj>
  Matrix(Vector<R> vec, Proj proj)
    : m_Cols(vec.size() == 0 ? 0 : vec[0].size())
    , m_Rows(vec.size())
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
      auto rowTransformed = std::views::transform(row, proj);
      std::ranges::copy(rowTransformed, std::back_inserter(m_Data));
    }
  }

  explicit Matrix(Dimension dimension, const T& initialValue)
    : m_Cols(dimension.cols)
    , m_Rows(dimension.rows)
  {
    if (m_Rows == 0 || m_Cols == 0) {
      throw std::runtime_error("Matrix dimensions cannot be zero");
    }
    m_Data.resize(m_Rows * m_Cols, initialValue);
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

  struct PositionValue
  {
    std::size_t x;
    std::size_t y;
    T value;
  };

  auto find(const T& value) const
  {
    std::vector<PositionValue> results;
    for (std::size_t y = 0; y < m_Rows; ++y) {
      for (std::size_t x = 0; x < m_Cols; ++x) {
        if (operator[](x, y) == value) {
          results.push_back(PositionValue{ x, y, value });
        }
      }
    }
    return results;
  }

  auto count(const T& value) const { return std::ranges::count(m_Data, value); }

  template<typename F>
  void print(F f) const
  {
    for (std::size_t y = 0; y < m_Rows; ++y) {
      for (std::size_t x = 0; x < m_Cols; ++x) {
        std::print("{} ", f(operator[](x, y)));
      }
      std::print("\n");
    }
  }

  [[nodiscard]] std::size_t height() const { return m_Rows; }
  [[nodiscard]] std::size_t width() const { return m_Cols; }
};

}