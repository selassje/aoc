#include "day17.hpp"

#include <cstddef>
#include <variant>

namespace aoc22::day17 {

struct Point
{
  std::size_t x;
  std::size_t y;
};

struct Line
{
  Point start;
  Point end;
  constexpr bool isVertical() const noexcept { return start.x == end.x; }
  bool isOverlapping(const Line& line) const noexcept
  {
    static constexpr auto inRange =
      [](const auto& min, const auto& val, const auto& max) {
        return min <= val && val <= max;
      };

    if (isVertical() == line.isVertical()) {
      if (isVertical()) {
        return start.x == line.start.x &&
               (inRange(start.y, line.start.y, end.y) ||
                inRange(start.y, line.end.y, end.y));
      }
      return start.y == line.start.y &&
             (inRange(start.x, line.start.x, end.x) ||
              inRange(start.x, line.end.x, end.x));
    } else {
      if (isVertical()) {
        return inRange(line.start.x, start.x, line.end.x) &&
               inRange(start.y, line.start.y, end.y);
      }
      return inRange(line.start.y, start.y, line.end.y) &&
             inRange(start.x, line.start.x, end.x);
    }
  }
};

using Lines = std::vector<Line>;

struct RockCommon
{
  Point bottomLeft;
};

struct HorizontalLine : RockCommon
{};
struct Cross : RockCommon
{};
struct ReversedL : RockCommon
{};
struct VerticalLine : RockCommon
{};
struct Square : RockCommon
{};

struct GetLines
{

  Lines operator()(const HorizontalLine& rock)
  {
    return { { { rock.bottomLeft.x, rock.bottomLeft.y },
               { rock.bottomLeft.x + 3, rock.bottomLeft.y } } };
  }

  Lines operator()(const Cross& rock)
  {
    const Line horizontal = { { rock.bottomLeft.x, rock.bottomLeft.y + 1 },
                              { rock.bottomLeft.x + 2,
                                rock.bottomLeft.y + 1 } };
    const Line vertical = { { rock.bottomLeft.x + 1, rock.bottomLeft.y },
                            { rock.bottomLeft.x + 1, rock.bottomLeft.y + 2 } };
    return { horizontal, vertical };
  }

  Lines operator()(const ReversedL& rock)
  {
    const Line horizontal = { { rock.bottomLeft.x, rock.bottomLeft.y },
                              { rock.bottomLeft.x + 2, rock.bottomLeft.y } };
    const Line vertical = { { rock.bottomLeft.x + 2, rock.bottomLeft.y },
                            { rock.bottomLeft.x + 2, rock.bottomLeft.y + 2 } };
    return { horizontal, vertical };
  }

  Lines operator()(const VerticalLine& rock)
  {
    return { { { rock.bottomLeft.x, rock.bottomLeft.y },
               { rock.bottomLeft.x, rock.bottomLeft.y + 3 } } };
  }

  Lines operator()(const Square& rock)
  {
    const Line horizontal1 = { { rock.bottomLeft.x, rock.bottomLeft.y },
                               { rock.bottomLeft.x + 1, rock.bottomLeft.y } };
    const Line horizontal2 = { { rock.bottomLeft.x, rock.bottomLeft.y + 1 },
                               { rock.bottomLeft.x + 1,
                                 rock.bottomLeft.y + 1 } };
    return { horizontal1, horizontal2 };
  }
};
struct GetRightEdge
{
  auto operator()(const HorizontalLine& rock) { return rock.bottomLeft.x + 3; }

  auto operator()(const Cross& rock) { return rock.bottomLeft.x + 2; }

  auto operator()(const ReversedL& rock) { return rock.bottomLeft.x + 2; }

  auto operator()(const VerticalLine& rock) { return rock.bottomLeft.x; }

  auto operator()(const Square& rock) { return rock.bottomLeft.x + 1; }
};
struct GetHeight
{
  auto operator()(const HorizontalLine&) { return 1U; }

  auto operator()(const Cross&) { return 3U; }

  auto operator()(const ReversedL&) { return 3U; }

  auto operator()(const VerticalLine&) { return 4U; }

  auto operator()(const Square&) { return 2U; }
};

using Rock =
  std::variant<HorizontalLine, Cross, ReversedL, VerticalLine, Square>;

struct MoveLeft
{};
struct MoveRight
{};

using Move = std::variant<MoveLeft, MoveRight>;

auto moveSideways = []<typename R, typename M>(R& rock, M&) {
  auto movedRock = rock;
  if constexpr (std::is_same<std::decay_t<M>, MoveLeft>::value == true) {
    if (movedRock.bottomLeft.x > 1) {
      movedRock.bottomLeft.x -= 1;
    }
  } else {
    const auto rightEdge = std::visit(GetRightEdge{}, Rock{ movedRock });
    static constexpr std::size_t chamberWidth = 7;
    if (rightEdge < chamberWidth) {
      movedRock.bottomLeft.x += 1;
    }
  }
  return Rock{ movedRock };
};

auto moveDown = []<typename R>(R& rock) {
  auto movedRock = rock;
  movedRock.bottomLeft.y -= 1;
  return Rock{ movedRock };
};

auto getBottom = []<typename R>(R& rock) { return rock.bottomLeft.y; };

bool
isMovePossible(const std::vector<Rock>& rocks, const Rock& movedRock)
{
  const auto lines2 = std::visit(GetLines{}, movedRock);
  for (const auto& rock : rocks) {
    const auto lines1 = std::visit(GetLines{}, rock);
    for (const auto& line1 : lines1) {
      for (const auto& line2 : lines2)
        if (line1.isOverlapping(line2)) {
          return false;
        }
    }
  }
  return true;
}

Result
solve(const Input& input)
{
  auto getNextRock = [i = static_cast<std::size_t>(-1)](
                       const Point& leftBottomEdge) mutable -> Rock {
    i = (i + 1) % std::variant_size_v<Rock>;
    switch (i) {
      case 0:
        return HorizontalLine{ leftBottomEdge };
      case 1:
        return Cross{ leftBottomEdge };
      case 2:
        return ReversedL{ leftBottomEdge };
      case 3:
        return VerticalLine{ leftBottomEdge };
      case 4:
        return Square{ leftBottomEdge };
      default:
        std::abort();
    }
  };
  static constexpr std::size_t targetStoppedRocksCount = 2022;
  const std::size_t jetCount = input.size();

  std::size_t heighestRock = 0;
  std::vector<Rock> stoppedRocks{};
  static constexpr std::size_t leftEdge = 3;
  std::size_t i = 0;
  while (stoppedRocks.size() < targetStoppedRocksCount) {
    auto rock = getNextRock({ leftEdge, heighestRock + 4 });
    while (true) {
      const auto jet =
        input[i] == JetDir::Left ? Move{ MoveLeft{} } : Move{ MoveRight{} };
      i = (i + 1) % jetCount;
      const auto movedRock = std::visit(moveSideways, rock, jet);
      if (isMovePossible(stoppedRocks, movedRock)) {
        rock = std::move(movedRock);
      }
      const auto movedDownRock = std::visit(moveDown, rock);
      const auto bottom = std::visit(getBottom, movedDownRock);
      if (bottom == 0 || !isMovePossible(stoppedRocks, movedDownRock)) {
        const auto height = std::visit(GetHeight{}, movedDownRock);
        heighestRock = std::max(heighestRock, bottom + height);
        break;
      } else {
        rock = std::move(movedDownRock);
      }
    }
    stoppedRocks.emplace_back(rock);
  }

  return { heighestRock, heighestRock };
}

}