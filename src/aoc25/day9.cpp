module aoc25.day9;

using aoc25::day9::Input;
using aoc25::day9::Point;
using aoc25::day9::Result;

struct PointHash
{
  std::size_t operator()(const Point& p) const noexcept
  {
    const auto h1 = static_cast<std::size_t>(p.x);
    const auto h2 = static_cast<std::size_t>(p.y);
    return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
  }
};

enum class Tile : std::uint8_t
{
  Empty,
  Red,
  Green
};

using Points = std::vector<Point>;

struct Rectangle
{
  Point topLeft;
  Point bottomRight;
};

struct Wall
{
  Point start;
  Point end;
};

using Walls = std::vector<Wall>;

using Grid = std::vector<std::vector<bool>>;
using PrefixSumGrid = std::vector<std::vector<std::uint64_t>>;
namespace {

auto
getArea(const Point& a, const Point& b)
{
  return (std::max(a.x, b.x) - std::min(a.x, b.x) + 1) *
         (std::max(a.y, b.y) - std::min(a.y, b.y) + 1);
}

auto
floodFindExteriorPoints(const Grid& grid)
{
  const auto nY = grid.size();
  const auto nX = grid[0].size();
  std::queue<Point> toVisit{};
  Grid visited(nY, std::vector<bool>(nX, false));
  toVisit.push({ 0, 0 });
  while (!toVisit.empty()) {
    const auto current = toVisit.front();
    toVisit.pop();
    if (visited[current.y][current.x]) {
      continue;
    }
    visited[current.y][current.x] = true;
    const std::vector<Point> neighbors = {
      { current.x + 1, current.y },
      { current.x - 1, current.y },
      { current.x, current.y + 1 },
      { current.x, current.y - 1 },
    };
    for (const auto& neighbor : neighbors) {
      if (neighbor.x >= nX || neighbor.y >= nY) {
        continue;
      }
      if (grid[neighbor.y][neighbor.x]) {
        continue;
      }
      toVisit.push(neighbor);
    }
  }
  return visited;
}

auto
getCompressedCoordinates(const Input& input, const Rectangle& margin)
{
  std::set<std::uint64_t> xCoordsSet{};
  std::set<std::uint64_t> yCoordsSet{};
  for (const auto& point : input) {
    xCoordsSet.insert(point.x);
    yCoordsSet.insert(point.y);
  }
  xCoordsSet.insert(margin.topLeft.x - 1);
  xCoordsSet.insert(margin.bottomRight.x + 1);
  yCoordsSet.insert(margin.topLeft.y - 1);
  yCoordsSet.insert(margin.bottomRight.y + 1);

  std::map<std::uint64_t, std::size_t> xCoordMap{};
  std::map<std::uint64_t, std::size_t> yCoordMap{};

  std::size_t index = 0;
  for (const auto x : xCoordsSet) {
    xCoordMap[x] = index++;
  }

  index = 0;
  for (const auto y : yCoordsSet) {
    yCoordMap[y] = index++;
  }

  return std::make_pair(xCoordMap, yCoordMap);
}

auto
calculatePrefixSumGrid(const Grid& input)
{
  const auto nY = input.size();
  const auto nX = input[0].size();
  PrefixSumGrid prefixSum(nY, std::vector<std::uint64_t>(nX, 0));

  for (std::size_t y = 0; y < nY; ++y) {
    for (std::size_t x = 0; x < nX; ++x) {
      const auto value = input[y][x] ? 1ULL : 0ULL;
      auto& sum = prefixSum[y][x];
      sum = value;
      if (x > 0) {
        sum += prefixSum[y][x - 1];
      }
      if (y > 0) {
        sum += prefixSum[y - 1][x];
      }
      if (x > 0 && y > 0) {
        sum -= prefixSum[y - 1][x - 1];
      }
    }
  }
  return prefixSum;
}

auto
getRectangleSum(const Rectangle& rectangle, const PrefixSumGrid& prefixSum)
{
  const auto topLeftX = rectangle.topLeft.x;
  const auto topLeftY = rectangle.topLeft.y;
  const auto bottomRightX = rectangle.bottomRight.x;
  const auto bottomRightY = rectangle.bottomRight.y;

  std::uint64_t sum = prefixSum[bottomRightY][bottomRightX];
  if (topLeftX > 0) {
    sum -= prefixSum[bottomRightY][topLeftX - 1];
  }
  if (topLeftY > 0) {
    sum -= prefixSum[topLeftY - 1][bottomRightX];
  }
  if (topLeftX > 0 && topLeftY > 0) {
    sum += prefixSum[topLeftY - 1][topLeftX - 1];
  }
  return sum;
}

auto
getMaxAreas(const Input& input)
{
  std::uint64_t minX = std::numeric_limits<std::uint64_t>::max();
  std::uint64_t maxX = 0;
  std::uint64_t minY = std::numeric_limits<std::uint64_t>::max();
  std::uint64_t maxY = 0;
  for (std::size_t i = 0; i < input.size(); ++i) {
    for (std::size_t j = i + 1; j < input.size(); ++j) {
      const auto& a = input[i];
      const auto& b = input[j];
      minX = std::min({ minX, a.x, b.x });
      maxX = std::max({ maxX, a.x, b.x });
      minY = std::min({ minY, a.y, b.y });
      maxY = std::max({ maxY, a.y, b.y });
    }
  }
  const auto margin = Rectangle{ Point{ minX, minY }, Point{ maxX, maxY } };

  auto [compressedX, compressedY] = getCompressedCoordinates(input, margin);
  const auto nX = compressedX.size();
  const auto nY = compressedY.size();

  Walls walls{};
  for (std::size_t i = 0; i < input.size(); ++i) {
    const auto& redTilePoint = input[i];
    const auto& nextRedTilePoint = input[(i + 1) % input.size()];
    minX = std::min(redTilePoint.x, nextRedTilePoint.x);
    maxX = std::max(redTilePoint.x, nextRedTilePoint.x);
    minY = std::min(redTilePoint.y, nextRedTilePoint.y);
    maxY = std::max(redTilePoint.y, nextRedTilePoint.y);
    walls.push_back(Wall{ { minX, minY }, { maxX, maxY } });
  }

  Grid grid(nY, std::vector<bool>(nX, false));

  for (auto& wall : walls) {
    for (auto x = compressedX[wall.start.x]; x <= compressedX[wall.end.x];
         ++x) {
      for (auto y = compressedY[wall.start.y]; y <= compressedY[wall.end.y];
           ++y) {
        grid[y][x] = true;
      }
    }
  }

  const auto exteriorPoints = floodFindExteriorPoints(grid);
  const auto prefixSumGrid = calculatePrefixSumGrid(exteriorPoints);
  auto compress = [&](const Point& p) {
    return Point{ compressedX[p.x], compressedY[p.y] };
  };

  std::uint64_t maxAreaPart1 = 0;
  std::uint64_t maxAreaPart2 = 0;
  for (std::size_t i = 0; i < input.size(); ++i) {
    for (std::size_t j = i + 1; j < input.size(); ++j) {
      const auto compressedI = compress(input[i]);
      const auto compressedJ = compress(input[j]);
      const auto topLeft = Point{ std::min(compressedI.x, compressedJ.x),
                                  std::min(compressedI.y, compressedJ.y) };
      const auto bottomRight = Point{ std::max(compressedI.x, compressedJ.x),
                                      std::max(compressedI.y, compressedJ.y) };

      const auto rectangleSum =
        getRectangleSum(Rectangle{ topLeft, bottomRight }, prefixSumGrid);
      const auto area = getArea(input[i], input[j]);
      maxAreaPart1 = std::max(area, maxAreaPart1);
      if (area > maxAreaPart2 && rectangleSum == 0) {
        maxAreaPart2 = area;
      }
    }
  }
  return Result{ maxAreaPart1, maxAreaPart2 };
}
}
namespace aoc25::day9 {

Result
solve(const Input& input)
{
  return getMaxAreas(input);
}

}
