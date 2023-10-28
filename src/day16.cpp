#include "day16.hpp"

#include <algorithm>
#include <cstddef>
#include <deque>
#include <iterator>
#include <limits>
#include <map>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace aoc22::day16 {

struct ValveEx : Valve
{
  [[nodiscard]] std::size_t index() const noexcept
  {
    static constexpr std::size_t divider = 'Z' - 'A' + 1;
    auto toIndex = [](const char c) {
      return static_cast<std::size_t>(c - 'A');
    };
    return toIndex(name[0]) * divider + toIndex(name[1]);
  }
};

class InputEx
{
public:
  explicit InputEx(const Input& input)
  {
    m_input.reserve(input.size());
    std::ranges::transform(input,
                           std::back_inserter(m_input),
                           [](const auto& v) { return ValveEx{ v }; });
  }
  const ValveEx& operator[](std::string_view name) const noexcept
  {
    return *std::ranges::find(m_input, name, &ValveEx::name);
  }

  const ValveEx& operator[](std::size_t index) const noexcept
  {
    static constexpr std::size_t divider = 'Z' - 'A' + 1;
    const char hi = static_cast<char>(index / divider + 'A');
    const char lo = static_cast<char>(index % divider + 'A');
    return (*this)[std::string{ hi, lo }];
  }

  [[nodiscard]] auto begin() const noexcept { return m_input.begin(); }
  [[nodiscard]] auto end() const noexcept { return m_input.end(); }

private:
  std::vector<ValveEx> m_input;
};

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

using DistanceMap = std::map<std::size_t, std::map<std::size_t, std::size_t>>;

std::size_t
maxPressureReleasePart1(std::size_t valve, // NOLINT
                        std::size_t minutes,
                        const std::set<std::size_t>& openedValves,
                        const DistanceMap& distMap,
                        const InputEx& input)
{
  std::vector<std::size_t> results{ 0 };
  auto openedValves_ = openedValves; // NOLINT
  openedValves_.insert(valve);
  const auto str = input[valve].name;
  const auto& distances = distMap.at(valve);
  for (const auto& [nextValve, d] : distances) {
    const auto flowRate = input[nextValve].flowRate;
    if (nextValve != valve && !openedValves.contains(nextValve)) {
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
                        std::size_t valve2, // NOLINT
                        std::size_t minutes1,
                        std::size_t minutes2,
                        const std::set<std::size_t>& openedValves,
                        const DistanceMap& distMap,
                        const InputEx& input)
{
  std::vector<std::size_t> results{ 0 };
  auto openedValves_ = openedValves; // NOLINT
  openedValves_.insert(valve1);
  openedValves_.insert(valve2);
  const auto& distances1 = distMap.at(valve1);
  const auto& distances2 = distMap.at(valve2);
  for (const auto& [nextValve1, d1] : distances1) {
    for (const auto& [nextValve2, d2] : distances2) {
      if (nextValve1 != valve1 && nextValve2 != valve2) {
        const auto flowRate1 = input[nextValve1].flowRate;
        const auto flowRate2 = input[nextValve2].flowRate;
        const auto minutesSpent1 = d1 + 1;
        const auto minutesSpent2 = d2 + 1;

        auto releasedPressure = [&openedValves_](auto valve,
                                                 auto minutes,
                                                 auto minutesSpent,
                                                 auto flowRate) {
          return !openedValves_.contains(valve) && minutes > minutesSpent
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

  static constexpr std::size_t totalMinutesPart1 = 30;
  std::size_t part1 = 0;
  part1 = maxPressureReleasePart1(0, totalMinutesPart1, {}, distances, inputEx);

  static constexpr std::size_t totalMinutesPart2 = 26;
  std::size_t part2 = 0;
  part2 = maxPressureReleasePart2(
    0, 0, totalMinutesPart2, totalMinutesPart2, {}, distances, inputEx);

  return { part1, part2 };
}
};