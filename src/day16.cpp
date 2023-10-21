#include "day16.hpp"

#include <algorithm>
#include <array>
#include <numeric>
#include <optional>
#include <ranges>
#include <string>

namespace aoc22::day16 {

constexpr std::size_t
valveIndex(std::string_view name)
{
  auto toIndex = [](const char c) { return static_cast<std::size_t>(c - 'A'); };
  return toIndex(name[0]) * toIndex(name[1]);
};

constexpr auto maxValves = valveIndex("ZZ") + 1;
constexpr std::size_t totalMinutes = 30;
constexpr std::size_t actionMove = 0;
constexpr std::size_t actionOpen = 1;
constexpr std::size_t actionNone = 2;

constexpr std::string
indexToName(std::size_t index)
{
  const std::size_t divider = 'Z' - 'A' + 1;
  const char second = static_cast<char>(index / divider) + 'A';  
  const char first = static_cast<char>(index % divider) + 'A';
  return std::string{second,first};
};

using Dp = std::optional<std::size_t>[totalMinutes][maxValves][actionNone + 1];

Valve&
getValve(std::string_view name, Input& input)
{
  const auto it = std::ranges::find(input, name, &Valve::name);
  return *it;
}
bool
isValve(std::string_view name, const Input& input)
{
  const auto it = std::ranges::find(input, name, &Valve::name);
  return it != input.end();
}

const Valve&
getValve(std::string_view name, const Input& input)
{
  return *std::ranges::find(input, name, &Valve::name);
}

std::size_t
maxPressureRelease(std::string valveName,
                   std::size_t minutes,
                   const Input& input)
{
  if (minutes == 1) {
    return 0;
  } else if (minutes == 2) {
    return getValve(valveName, input).flowRate;
  } else {
    std::vector<std::size_t> subResults{ 0 };
    if (getValve(valveName, input).flowRate != 0) {
      Input newInput = input;
      auto& flowRate = getValve(valveName, newInput).flowRate;
      auto openingResult = (minutes - 1) * flowRate;
      flowRate = 0;
      openingResult += maxPressureRelease(valveName, minutes - 1, newInput);
      subResults.push_back(openingResult);
    }
    {
      for (const auto& connectedValve :
           getValve(valveName, input).connectedValves) {
        // if ( getValve(connectedValve, input).flowRate != 0)
        {
          const auto moveResult =
            maxPressureRelease(connectedValve, minutes - 1, input);
          subResults.push_back(moveResult);
        }
      }
    }
    return std::ranges::max(subResults);
  }
}

std::size_t getDp(Dp& dp,std::size_t m, std::size_t v, std::size_t a) {
  const auto d = dp[m][v][a];
  return d ? *d : 0;
}

Result
solve(const Input& input)
{
  Dp dp{};
  dp[0][0][actionOpen] = 0;
  for (const auto& valve : getValve("AA", input).connectedValves) {
    dp[0][valveIndex(valve)][actionMove] = 0;
  }

  for (std::size_t minute = 0; minute < totalMinutes; ++minute) {
    for (std::size_t valve = 0; valve < maxValves; ++valve) {
      for (std::size_t action = 0; action <= actionNone; ++action) {
        auto& d = dp[minute][valve][action];
        if (minute == 0) {
          d = 0;
        } else if  (isValve(indexToName(valve),input))  {

          if (action == actionNone) {
              d = std::max({getDp(dp,minute - 1, valve, actionMove),
                        getDp(dp, minute - 1, valve, actionOpen),
                        getDp(dp,minute - 1, valve, actionNone )});
          }
          if (action == actionOpen) {
            try {
            d =
              std::max({getDp(dp,minute - 1, valve, actionMove),
                        getDp(dp, minute - 1, valve, actionOpen),
                        getDp(dp,minute - 1, valve, actionNone )}) +
              getValve(indexToName(valve), input).flowRate * (totalMinutes - minute - 1);
              auto t = d;
              auto str = indexToName(valve);
              auto x = t;
            } catch(...) {
              d = 0;
            }
          }
          if (action == actionMove) {
            const auto prev =
              std::max({getDp(dp,minute - 1, valve, actionMove),
                        getDp(dp, minute - 1, valve, actionOpen),
                        getDp(dp,minute - 1, valve, actionNone )});
          }

        }
      }
    }
  }
  std::size_t part1 = 0;
  // part1 = maxPressureRelease("AA", 10, input);
  for (std::size_t minute = 0; minute < totalMinutes; ++minute) {
    for (std::size_t valve = 0; valve < maxValves; ++valve) {
      const auto d = dp[minute][valve][actionOpen];
      if (d) {
        part1 = std::max(part1, *d);
      }
    }
  }

  return { part1, part1 };
}
};