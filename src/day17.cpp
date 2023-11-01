#include "day17.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace aoc22::day17 {

struct Point
{
  std::size_t x;
  std::size_t y;
};

enum Rock : std::size_t
{
  HorizontalLine = 0,
  Cross = 1,
  ReversedL = 2,
  VerticalLine = 3,
  Square = 4,
  Count = 5
};

inline constexpr std::byte
operator"" _B(unsigned long long arg) noexcept // NOLINT
{
  return static_cast<std::byte>(arg);
}

class Tower
{
public:
  Tower() { resizeForNextRock(); }

  void setBitMapAt(Point bottomLeft, Rock type)
  {
    const auto typeIndex = static_cast<std::size_t>(type);
    const auto val = ROCK_INFOS[typeIndex].bitMap;
    const int shift = 4 - static_cast<int>(bottomLeft.x);
    for (std::size_t bitMapRowIndex = 0; bitMapRowIndex < 4; ++bitMapRowIndex) {
      const auto valShift = static_cast<std::uint16_t>(4 * bitMapRowIndex);
      static constexpr std::uint16_t nibble = 0xF;
      const auto y = bottomLeft.y + bitMapRowIndex;
      const auto nibbleShift = static_cast<std::uint16_t>(nibble << valShift);
      auto row = static_cast<std::byte>(
        static_cast<std::uint16_t>(val & nibbleShift) >> valShift);
      row = shift >= 0 ? row << shift : row >> std::abs(shift);
      m_rows[y] = m_rows[y] | row;
    }
    m_height =
      std::max(m_height, bottomLeft.y + ROCK_INFOS[typeIndex].height - 1);
    resizeForNextRock();
    ++m_rocksCount;
  }

  bool isCollision(Point bottomLeft, Rock type)
  {
    const auto typeIndex = static_cast<std::size_t>(type);
    return bottomLeft.y == 0 || bottomLeft.x == 0 ||
           bottomLeft.x + ROCK_INFOS[typeIndex].width > ROW_WIDTH + 1 ||
           (getBitMapAt(bottomLeft) & ROCK_INFOS[typeIndex].bitMap) != 0;
  }

  [[nodiscard]] std::size_t height() const { return m_height; }
  [[nodiscard]] std::size_t rocksCount() const { return m_rocksCount; }

private:
  void resizeForNextRock() { m_rows.resize(m_rows.size() + 8); }

  [[nodiscard]] std::uint16_t getBitMapAt(Point bottomLeft) const
  {
    std::uint16_t result = 0;
    const int shift = 4 - static_cast<int>(bottomLeft.x);
    for (std::size_t bitMapRowIndex = 0; bitMapRowIndex < 4; ++bitMapRowIndex) {
      const auto y = bottomLeft.y + bitMapRowIndex;
      auto row =
        static_cast<std::uint16_t>(m_rows[y] & ROW_MASKS[bottomLeft.x - 1]);
      row = shift >= 0 ? row >> static_cast<std::uint16_t>(shift)
                       : static_cast<std::uint16_t>(
                           row << static_cast<std::uint16_t>(abs(shift)));
      result += static_cast<std::uint16_t>(row << 4 * bitMapRowIndex);
    }
    return result;
  }
  std::size_t m_height{};
  std::vector<std::byte> m_rows{};
  std::size_t m_rocksCount{};
  static constexpr inline std::size_t ROW_WIDTH = 7;

  static constexpr inline std::array<std::byte, ROW_WIDTH> ROW_MASKS = {
    0b01111000_B, 0b00111100_B, 0b00011110_B, 0b00001111_B,
    0b00000111_B, 0b00000011_B, 0b00000001_B
  };

  struct RockInfo
  {
    std::size_t height;
    std::size_t width;
    std::uint16_t bitMap;
  };

  static constexpr inline std::array<RockInfo, Rock::Count> ROCK_INFOS = {
    RockInfo{ 1, 4, 0b0000000000001111 },
    { 3, 3, 0b0000010011100100 },
    { 3, 3, 0b0000001000101110 },
    { 4, 1, 0b1000100010001000 },
    { 2, 2, 0b0000000011001100 }
  };
};

Result
solve(const Input& input)
{
  static constexpr std::size_t targetStoppedRocksCountPart1 = 2022;
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
      }
      --y;
    }
    rock = static_cast<Rock>((rock + 1) % Rock::Count);
  }
  return { tower.height(), tower.height() };
}
}