#include "day16.hpp"

#include <algorithm>
#include <array>
#include <bitset>
#include <deque>
#include <map>
#include <numeric>
#include <optional>
#include <ranges>
#include <set>
#include <string>
#include <vector>

namespace aoc22::day16 {

static constexpr std::size_t letterCount = 'Z' - 'A' + 1;
static constexpr std::size_t maxValveCount = letterCount * letterCount;

using ValveSet = std::bitset<maxValveCount>;

struct ValveEx : Valve
{
  [[nodiscard]] std::size_t index() const noexcept
  {
    auto toIndex = [](const char c) {
      return static_cast<std::size_t>(c - 'A');
    };
    return toIndex(name[0]) * letterCount + toIndex(name[1]);
  }
};

class InputEx
{
public:
  explicit InputEx(const Input& input)
  {
    mInput.reserve(input.size());
    std::ranges::transform(input,
                           std::back_inserter(mInput),
                           [](const auto& v) { return ValveEx{ v }; });
  }
  const ValveEx& operator[](std::string_view name) const noexcept
  {
    return *std::ranges::find(mInput, name, &ValveEx::name);
  }

  const ValveEx& operator[](std::size_t index) const noexcept
  {
    const char hi = static_cast<char>(index / letterCount) + 'A';
    const char lo = static_cast<char>(index % letterCount) + 'A';
    return (*this)[std::string{ hi, lo }];
  }

  auto begin() const noexcept { return mInput.begin(); }
  auto end() const noexcept { return mInput.end(); }

private:
  std::vector<ValveEx> mInput;
};

using DistanceMap = std::map<std::size_t, std::map<std::size_t, std::size_t>>;

using DistancePair = std::pair<std::size_t,std::size_t>;

using DistanceMap2 = std::map<std::size_t, std::vector<DistancePair>>;

auto
findShortestPath(std::size_t src, const InputEx& input)
{
  static constexpr auto maxDistance =
    std::numeric_limits<decltype(Result::part1)>::max();

  std::set<std::size_t> visitedValves{};
  std::deque<std::size_t> toBeVisitedValves{};
  std::map<std::size_t, std::size_t> distances{};

  for (const auto& valve : input) {
    const auto valveIndex = valve.index();
    if (valveIndex == src) {
      distances[valveIndex] = 0;
    } else {
      distances[valveIndex] = maxDistance;
    }
  }

  toBeVisitedValves.push_back(src);
  while (!toBeVisitedValves.empty()) {
    const auto valveIndex = toBeVisitedValves.front();
    const auto& valve = input[valveIndex];
    for (const auto& neighbour : valve.connectedValves) {
      const auto neighbourIndex = input[neighbour].index();
      if (!visitedValves.contains(neighbourIndex)) {
        const auto neighbourValve = input[neighbour];
        if (const auto canidateLength = distances[valveIndex] + 1;
            canidateLength < distances[neighbourIndex]) {
          distances[neighbourIndex] = canidateLength;
        }
        toBeVisitedValves.push_back(neighbourIndex);
      }
    }
    visitedValves.insert(valveIndex);
    toBeVisitedValves.pop_front();
  }
  auto nonZeroDistances =
    distances | std::views::filter([&input](const auto& pair) {
      return input[pair.first].flowRate > 0;
    });
  return std::map<std::size_t, std::size_t>(nonZeroDistances.begin(),
                                            nonZeroDistances.end());
}


std::size_t
maxPressureReleasePart1(std::size_t valve,
                        std::size_t minutes,
                        const ValveSet& openedValves,
                        const DistanceMap& distMap,
                        const InputEx& input)
{
  std::vector<std::size_t> results{ 0 };
  auto openedValves_ = openedValves;
  openedValves_.set(valve, true);
  const auto str = input[valve].name;
  const auto& distances = distMap.at(valve);
  for (const auto& [nextValve, d] : distances) {
    const auto flowRate = input[nextValve].flowRate;
    if (nextValve != valve && !openedValves.test(nextValve)) {
      const auto minutesSpent = d + 1;
      if (minutes > minutesSpent) {
        const auto releasedPressure =
          flowRate * (minutes - minutesSpent) +
          maxPressureReleasePart1(
            nextValve, minutes - minutesSpent, openedValves_, distMap, input);
        results.push_back(releasedPressure);
      }
    }
  }
  return std::ranges::max(results);
}
std::size_t
maxPressureReleasePart2(std::size_t valve1,
                        std::size_t valve2,
                        std::size_t minutes1,
                        std::size_t minutes2,
                        const ValveSet& openedValves,
                        const DistanceMap& distMap,
                        const InputEx& input)
{
  std::vector<std::size_t> results{ 0 };
  auto openedValves_ = openedValves;
  openedValves_.set(valve1, true);
  openedValves_.set(valve2, true);
  const auto& distances1 = distMap.at(valve1);
  const auto& distances2 = distMap.at(valve2);
  for (const auto& [nextValve1, d1] : distances1) {
    for (const auto& [nextValve2, d2] : distances2) {
      if (nextValve1 != valve1 && nextValve2 != valve2 &&
          !openedValves_.test(nextValve1) && !openedValves_.test(nextValve2)) {
        const auto flowRate1 = input[nextValve1].flowRate;
        const auto flowRate2 = input[nextValve2].flowRate;
        const auto minutesSpent1 = d1 + 1;
        const auto minutesSpent2 = d2 + 1;

        auto releasedPressure = [&openedValves_](auto valve,
                                                 auto minutes,
                                                 auto minutesSpent,
                                                 auto flowRate) {
          return !openedValves_.test(valve) && minutes > minutesSpent
                   ? flowRate * (minutes - minutesSpent)
                   : 0;
        };

        const std::size_t releasedPressure1 =
          releasedPressure(nextValve1, minutes1, minutesSpent1, flowRate1);
        const std::size_t releasedPressure2 =
          releasedPressure(nextValve2, minutes2, minutesSpent2, flowRate2);
        if (releasedPressure1 != 0 && releasedPressure2 != 0) {
          auto totalReleasedPressure =
            maxPressureReleasePart2(nextValve1,
                                    nextValve2,
                                    minutes1 - minutesSpent1,
                                    minutes2 - minutesSpent2,
                                    openedValves_,
                                    distMap,
                                    input);
          if (nextValve1 != nextValve2) {
            totalReleasedPressure += releasedPressure2 + releasedPressure1;
          } else {
            totalReleasedPressure +=
              std::max({ releasedPressure1, releasedPressure2 });
          }
          results.push_back(totalReleasedPressure);
        }
      }
    }
  }
  return std::ranges::max(results);
}

Result
solve(const Input& input)
{
  const InputEx inputEx{ input };
  std::map<std::size_t, std::map<std::size_t, std::size_t>> distances;
  for (const auto& valve : inputEx) {
    const auto valveIndex = valve.index();
    if (valveIndex == 0 || valve.flowRate > 0) {
      distances[valveIndex] = findShortestPath(valveIndex, inputEx);
    }
  }

  std::size_t part1 = 0;
  part1 = maxPressureReleasePart1(0, 30, {}, distances, inputEx);

  std::size_t part2 = 0;
  part2 = maxPressureReleasePart2(0, 0, 26, 26, {}, distances, inputEx);

  return { part1, part2 };
}
};