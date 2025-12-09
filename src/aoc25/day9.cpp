module aoc25.day9;

using aoc25::day9::Point;

auto
getArea(const Point& a, const Point& b)
{
  return (std::max(a.x, b.x) - std::min(a.x, b.x) + 1) *
         (std::max(a.y, b.y) - std::min(a.y, b.y) + 1);
}

namespace aoc25::day9 {

Result
solve(const Input& input)
{
  std::uint64_t part1 = 0;

  for (std::size_t i = 0; i < input.size(); ++i) {
    for (std::size_t j = i + 1; j < input.size(); ++j) {
      part1 = std::max(getArea(input[i], input[j]), part1);
    }
  }
  std::uint64_t part2 = part1;
  return { part1, part2 };
}

}