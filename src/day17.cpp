#include "day17.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <variant>

namespace aoc22::day17 {

struct Point
{
  std::size_t x;
  std::size_t y;
};

enum class Rock : std::size_t
{
  HorizontalLine = 0,
  Cross = 1,
  ReversedL = 2,
  VerticalLine = 3,
  Square = 4,
  Count = 5
};

constexpr std::array<std::uint16_t, static_cast<std::size_t>(Rock::Count)>
  rocksAsIntegers = { 0b0000000000001111,
                      0b0000010011100100,
                      0b0000001000101110,
                      0b1000100010001000,
                      0b0000000011001100 };

constexpr std::array<std::size_t, static_cast<std::size_t>(Rock::Count)>
  rocksHeights = { 1, 3, 3, 4, 2 };

constexpr std::array<std::size_t, static_cast<std::size_t>(Rock::Count)>
  rocksWidth = { 4, 3, 3, 1, 2 };

inline constexpr std::byte
operator"" _B(unsigned long long arg) noexcept
{
  return static_cast<std::byte>(arg);
}

class Tower
{
public:
  void setBitMapAt(Point bottomLeft, Rock type)
  {
    const auto typeIndex = static_cast<std::size_t>(type);
    const auto val = rocksAsIntegers[typeIndex];
    const int shift = 4 - static_cast<int>(bottomLeft.x);
    for (std::size_t y = 0; y < 4; ++y) {
      const auto y_ = bottomLeft.y + y;
      auto row = static_cast<std::byte>((val & (0xF << 4 * y)) >> 4 * y);
      row = shift >= 0 ? row << shift : row >> std::abs(shift);
      rows[y_] = rows[y_] | row;
    }
    mHeight = std::max(mHeight, bottomLeft.y + rocksHeights[typeIndex] - 1);
    resize();
    ++mRocksCount;
  }

  bool isCollision(Point bottomLeft, Rock type)
  {
    const auto typeIndex = static_cast<std::size_t>(type);
    return bottomLeft.y == 0 || bottomLeft.x == 0 ||
           bottomLeft.x + rocksWidth[typeIndex] > 8 ||
           (getBitMapAt(bottomLeft) & rocksAsIntegers[typeIndex]);
  }

  std::size_t height() const { return mHeight; }
  std::size_t rocksCount() const { return mRocksCount; }

private:
  void resize() { rows.resize(rows.size() + 8); }

  std::uint16_t getBitMapAt(Point bottomLeft) const
  {
    std::uint16_t result = 0;
    const int shift = 4 - static_cast<int>(bottomLeft.x);
    for (std::size_t y = 0; y < 4; ++y) {
      const auto y_ = bottomLeft.y + y;
      std::uint16_t row =
        static_cast<std::uint16_t>(rows[y_] & rowMasks[bottomLeft.x - 1]);
      row = shift >= 0 ? row >> shift
                       : static_cast<std::uint16_t>(row << abs(shift));
      result += row << 4 * y;
    }
    return result;
  }
  std::size_t mHeight{};
  std::vector<std::byte> rows = std::vector<std::byte>(8);
  std::size_t mRocksCount{};
  static constexpr inline std::size_t rowWidth = 7;

  static constexpr inline std::byte rowMasks[] = { 0b01111000_B, 0b00111100_B,
                                                   0b00011110_B, 0b00001111_B,
                                                   0b00000111_B, 0b00000011_B,
                                                   0b00000001_B };
};

Result
solve(const Input& input)
{

  static constexpr std::size_t targetStoppedRocksCountPart1 = 2022;
  // static constexpr std::size_t targetStoppedRocksCountPart2 = 1000000000000;
  const std::size_t jetCount = input.size();
  Tower tower{};

  std::size_t i = 0;
  Rock rock = Rock::HorizontalLine;
  while (tower.rocksCount() < targetStoppedRocksCountPart1) {
    std::size_t x = 3;
    std::size_t y = tower.height() + 4;
    while (true) {
      const auto jet = input[i];
      i = (i + 1) % jetCount;

      if (jet == JetDir::Left) {
        if (!tower.isCollision({ x - 1, y }, rock)) {
          --x;
        }
      } else if (!tower.isCollision({ x + 1, y }, rock)) {
        ++x;
      }
      if (tower.isCollision({ x, y - 1 }, rock)) {
        tower.setBitMapAt({ x, y }, rock);
        break;
      } else {
        --y;
      }
    }
    rock = static_cast<Rock>((static_cast<std::size_t>(rock) + 1) %
                             static_cast<std::size_t>(Rock::Count));
  }
  return { tower.height(), tower.height() };
}
}