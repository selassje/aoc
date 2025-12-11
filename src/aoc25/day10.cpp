module aoc25.day10;

import std;

namespace {

using namespace aoc25::day10;

std::uint64_t
countMinimumPresses(const Machine& machine)
{
  const std::uint64_t statesCount = 1ULL << machine.lights.size();
  const std::uint64_t startState = 0;
  const std::uint64_t targetState = [&]() {
    std::uint64_t state = 0;
    for (std::size_t i = 0; i < machine.lights.size(); ++i) {
      if (machine.lights[i] == Light::On) {
        state |= (1ULL << i);
      }
    }
    return state;
  }();
  static constexpr auto inf = std::numeric_limits<std::uint64_t>::max();
  std::vector<std::uint64_t> distances(statesCount, inf);
  distances[startState] = 0;

  auto compare = [&distances](std::uint64_t lhs, std::uint64_t rhs) {
    if (distances[lhs] != distances[rhs]) {
      return distances[lhs] < distances[rhs];
    }
    return lhs < rhs;
  };

  std::set<std::uint64_t, decltype(compare)> queue(compare);
  queue.insert(startState);

  auto toggleLight = [](std::uint64_t state, std::size_t lightIndex) {
    return state ^ (1ULL << lightIndex);
  };

  while (!queue.empty()) {
    const std::uint64_t currentState = *queue.begin();
    queue.erase(queue.begin());
    for (std::size_t buttonIndex = 0; buttonIndex < machine.wirings.size();
         ++buttonIndex) {
      const auto& wiring = machine.wirings[buttonIndex];
      std::uint64_t nextState = currentState;
      for (std::size_t lightIndex = 0; lightIndex < wiring.size();
           ++lightIndex) {
        nextState =
          toggleLight(nextState, static_cast<std::size_t>(wiring[lightIndex]));
      }

      const std::uint64_t altDistance = distances[currentState] + 1;
      if (altDistance < distances[nextState]) {
        queue.erase(nextState);
        distances[nextState] = altDistance;
        queue.insert(nextState);
      }
    }
  }
  return distances[targetState];
}
}

namespace aoc25::day10 {

Result
solve(const Input& input)
{
  std::uint64_t part1 = 0;
  std::uint64_t part2 = 0;
  for (const auto& machine : input) {
    part1 += countMinimumPresses(machine);
  }
  part2 = part1;
  return { part1, part2 };
}
}