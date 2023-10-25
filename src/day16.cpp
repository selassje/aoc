#include "day16.hpp"

#include <algorithm>
#include <array>
#include <deque>
#include <map>
#include <numeric>
#include <optional>
#include <ranges>
#include <set>
#include <string>
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
    static constexpr std::size_t divider = 'Z' - 'A' + 1;
    const char hi = static_cast<char>(index / divider) + 'A';
    const char lo = static_cast<char>(index % divider) + 'A';
    return (*this)[std::string{ hi, lo }];
  }

  auto begin() const noexcept { return mInput.begin(); }
  auto end() const noexcept { return mInput.end(); }

private:
  std::vector<ValveEx> mInput;
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
  return distances;
}

using DistanceMap = std::map<std::size_t, std::map<std::size_t, std::size_t>>;

std::size_t
maxPressureReleasePart1(std::size_t valve,
                        std::size_t minutes,
                        const std::set<std::size_t>& openedValves,
                        const DistanceMap& distMap,
                        const InputEx& input)
{
  std::vector<std::size_t> results{ 0 };
  auto openedValves_ = openedValves;
  openedValves_.insert(valve);
  const auto& distances = distMap.at(valve);
  for (const auto& [k, d] : distances) {
    const auto& valveObj = input[k];
    if (k != valve && !openedValves.contains(k) && valveObj.flowRate > 0) {
      const auto minutesSpent = d + 1;
      if (minutes > minutesSpent) {
        const auto result =
          valveObj.flowRate * (minutes - minutesSpent) +
          maxPressureReleasePart1(
            k, minutes - minutesSpent, openedValves_, distMap, input);
        results.push_back(result);
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
                        const std::set<std::size_t>& openedValves,
                        const DistanceMap& distMap,
                        const InputEx& input)
{
  std::vector<std::size_t> results{ 0 };
  auto openedValves_ = openedValves;
  openedValves_.insert(valve1);
  openedValves_.insert(valve2);
  const auto& distances1 = distMap.at(valve1);
  const auto& distances2 = distMap.at(valve2);
  for (const auto& [k, d] : distances1) {
    for (const auto& [k2, d2] : distances2) {

      const auto flowRate1 = input[k].flowRate;
      const auto flowRate2 = input[k2].flowRate;
      if (k != valve1 && k2 != valve2 && flowRate1 > 0 && flowRate2 > 0) {

        const auto minutesSpent1 = d + 1;
        const auto minutesSpent2 = d2 + 1;
        std::size_t result1 = 0;
        std::size_t result2 = 0;
        if (k != valve1 && !openedValves_.contains(k) && flowRate1 > 0) {
          if (minutes1 > minutesSpent1) {
            result1 = flowRate1 * (minutes1 - minutesSpent1);
          }
        }
        if (k2 != valve2 && !openedValves_.contains(k2) && flowRate2 > 0) {
          if (minutes2 > minutesSpent2) {
            result2 = flowRate2 * (minutes2 - minutesSpent2);
          }
        }
        auto str1 = input[k].name;
        auto str2 = input[k2].name;
        const auto totalRes = result1 + result2;
        if (result1 != 0 && result2 != 0) {
          auto result = maxPressureReleasePart2(k,
                                                k2,
                                                minutes1 - minutesSpent1,
                                                minutes2 - minutesSpent2,
                                                openedValves_,
                                                distMap,
                                                input);
          if (k != k2) {
            result += result2 + result1;
          } else {
            result += std::max({ result1, result2 });
          }
          results.push_back(result);
        } else {
          int z = 3;
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