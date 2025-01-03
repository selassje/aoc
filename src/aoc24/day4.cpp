#include "aoc24/day4.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <ranges>
#include <vector>

namespace {

using Letter = aoc24::day4::Letter;
using Letters = std::vector<aoc24::day4::Letter>;
using LettersMatrix = std::vector<Letters>;

constexpr std::array<Letter, 4> XMAS = { Letter::X,
                                         Letter::M,
                                         Letter::A,
                                         Letter::S };

std::size_t
countXMAS(const Letters& letters)
{
  const std::size_t size = letters.size();
  if (size < 4) {
    return 0;
  }
  std::size_t result = 0;
  auto lettersStart = std::begin(letters);
  for (std::size_t i = 0; i <= size - 4; ++i) {
    auto lettersEnd = lettersStart;
    std::advance(lettersEnd, 4);
    if (std::ranges::equal(
          std::begin(XMAS), std::end(XMAS), lettersStart, lettersEnd)) {
      ++result;
    }
    ++lettersStart;
  }
  return result;
}

std::size_t
countXMASMatrix(const LettersMatrix& matrix)
{
  const auto rowsCounted = matrix | std::views::transform(countXMAS);
  return std::accumulate(
    std::begin(rowsCounted), std::end(rowsCounted), std::size_t{});
}

auto
getRowsReversed(const LettersMatrix& matrix)
{
  LettersMatrix rowsReversed = matrix;
  for (auto& row : rowsReversed) {
    std::ranges::reverse(row);
  }
  return rowsReversed;
}

template<bool REVERSE = false>
auto
getColumns(const LettersMatrix& matrix)
{
  const auto h = matrix.size();
  const auto w = matrix[0].size();
  LettersMatrix columns{};

  for (std::size_t x = 0; x < w; ++x) {
    Letters column{};
    column.resize(h);
    for (std::size_t y = 0; y < h; ++y) {
      if constexpr (REVERSE) {
        column[y] = matrix[h - 1 - y][x];
      } else {
        column[y] = matrix[y][x];
      }
    }
    columns.push_back(column);
  }
  return columns;
}

auto
getDiagonals(const LettersMatrix& matrix)
{
  LettersMatrix diagonals{};
  const auto h = matrix.size();
  const auto w = matrix[0].size();

  struct Quarter
  {
    std::size_t startX;
    std::size_t endX;
    std::size_t startY;
    std::size_t endY;
    bool incX;
    bool incY;
  };

  for (std::size_t i = 0; i < w; ++i) {
    Quarter quarters[] = { // NOLINT
                           { i, 0, h - 1, 0, false, false },
                           { w - i - 1, w - 1, 0, h - 1, true, true },
                           { w - i - 1, w - 1, h - 1, 0, true, false },
                           { i, 0, 0, h - 1, false, true }

    };

    for (std::size_t j = 0; j < 4; ++j) {
      const auto& quarter = quarters[j];
      std::size_t x = quarter.startX;
      std::size_t y = quarter.startY;
      Letters diagonal{};
      while ((i != w - 1) || j % 2 == 1) {
        diagonal.push_back(matrix[y][x]);
        if (y == quarter.endY || x == quarter.endX) {
          diagonals.push_back(diagonal);
          break;
        }
        if (quarter.incX) {
          ++x;
        } else {
          --x;
        }
        if (quarter.incY) {
          ++y;
        } else {
          --y;
        }
      }
    }
  }
  return diagonals;
}

auto
solvePart2(const aoc24::day4::Input& input)
{
  std::size_t result = 0;
  const auto h = input.size();
  const auto w = input[0].size();

  auto get = [&input](std::size_t x, std::size_t y) { return input[y][x]; };

  for (std::size_t y = 1; y < h - 1; ++y) {
    for (std::size_t x = 1; x < w - 1; ++x) {

      if (get(x, y) == Letter::A) {

        const bool isMas1 =
          (get(x - 1, y - 1) == Letter::M && get(x + 1, y + 1) == Letter::S) ||
          (get(x - 1, y - 1) == Letter::S && get(x + 1, y + 1) == Letter::M);
        const bool isMas2 =
          (get(x + 1, y - 1) == Letter::M && get(x - 1, y + 1) == Letter::S) ||
          (get(x + 1, y - 1) == Letter::S && get(x - 1, y + 1) == Letter::M);

        if (isMas1 && isMas2) {
          ++result;
        }
      }
    }
  }
  return result;
}

}
namespace aoc24::day4 {

Result
solve(const Input& input)
{
  std::size_t part1 = countXMASMatrix(input);
  part1 += countXMASMatrix(getRowsReversed(input));
  part1 += countXMASMatrix(getColumns(input));
  part1 += countXMASMatrix(getColumns<true>(input));
  const auto diagonals = getDiagonals(input);
  part1 += countXMASMatrix(diagonals);
  part1 += countXMASMatrix(getRowsReversed(diagonals));
  const std::size_t part2 = solvePart2(input);
  return { part1, part2 };
}

}