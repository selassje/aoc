#include "day17.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>

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
operator"" _B(std::uint64_t arg) noexcept
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
      const auto y = bottomLeft.y + bitMapRowIndex;
      auto row = static_cast<std::byte>((val & (0xF << 4 * bitMapRowIndex)) >>
                                        4 * bitMapRowIndex);
      row = shift >= 0 ? row << shift : row >> std::abs(shift);
      mRows[y] = mRows[y] | row;
    }
    mHeight = std::max(mHeight, bottomLeft.y + ROCK_INFOS[typeIndex].height - 1);
    resizeForNextRock();
    ++mRocksCount;
  }

  bool isCollision(Point bottomLeft, Rock type)
  {
    const auto typeIndex = static_cast<std::size_t>(type);
    return bottomLeft.y == 0 || bottomLeft.x == 0 ||
           bottomLeft.x + ROCK_INFOS[typeIndex].width > ROW_WIDTH + 1 ||
           (getBitMapAt(bottomLeft) & ROCK_INFOS[typeIndex].bitMap);
  }

  [[nodiscard]] std::size_t height() const { return mHeight; }
  [[nodiscard]] std::size_t rocksCount() const { return mRocksCount; }

private:
  void resizeForNextRock() { mRows.resize(mRows.size() + 8); }

  [[nodiscard]] std::uint16_t getBitMapAt(Point bottomLeft) const
  {
    std::uint16_t result = 0;
    const int shift = 4 - static_cast<int>(bottomLeft.x);
    for (std::size_t bitMapRowIndex = 0; bitMapRowIndex < 4; ++bitMapRowIndex) {
      const auto y = bottomLeft.y + bitMapRowIndex;
      auto row =
        static_cast<std::uint16_t>(mRows[y] & ROW_MASKS[bottomLeft.x - 1]);
      row = shift >= 0 ? row >> shift
                       : static_cast<std::uint16_t>(row << abs(shift));
      result += row << 4 * bitMapRowIndex;
    }
    return result;
  }
  std::size_t mHeight{};
  std::vector<std::byte> mRows{};
  std::size_t mRocksCount{};
  static constexpr inline std::size_t ROW_WIDTH = 7;

  static constexpr inline std::byte ROW_MASKS[] = { 0b01111000_B, 0b00111100_B,
                                                   0b00011110_B, 0b00001111_B,
                                                   0b00000111_B, 0b00000011_B,
                                                   0b00000001_B };
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