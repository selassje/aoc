#include "day16.hpp"

#include <algorithm>
#include <ranges>
#include <string>

namespace aoc22::day16 {

Valve&
getValve(std::string_view name, Input& input)
{
  return *std::ranges::find(input, name, &Valve::name);
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
    std::vector<std::size_t> subResults{0};
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
        const auto moveResult =
          maxPressureRelease(connectedValve, minutes - 1, input);
         subResults.push_back(moveResult);
      }
    }
    return std::ranges::max(subResults);
  }
}

Result
solve(const Input& input)
{
  const auto part1 = maxPressureRelease("AA", 30, input);
  return { part1, part1 };
}

};