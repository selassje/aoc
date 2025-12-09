module aoc25.day8;

import std;

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnull-dereference"
#endif
namespace {

using aoc25::day8::Input;
using aoc25::day8::Junction;
using Pair = std::pair<Junction, Junction>;
using Pairs = std::vector<Pair>;
using Circuit = std::set<Junction>;
using Circuits = std::vector<Circuit>;

double
getDistance(const Junction a, const Junction b)
{
  const auto dx = static_cast<double>(a.x) - static_cast<double>(b.x);
  const auto dy = static_cast<double>(a.y) - static_cast<double>(b.y);
  const auto dz = static_cast<double>(a.z) - static_cast<double>(b.z);
  return std::sqrt((dx * dx) + (dy * dy) + (dz * dz));
}

auto
getClosestPairs(const Input& input)
{
  const auto size = input.size();
  Pairs pairs{};
  pairs.reserve((size * (size - 1)) / 2);
  for (std::size_t i = 0; i < size; ++i) {
    for (std::size_t j = i + 1; j < size; ++j) {
      pairs.emplace_back(std::make_pair(input[i], input[j]));
    }
  }
  std::ranges::sort(pairs, [&](const Pair& a, const Pair& b) {
    return getDistance(a.first, a.second) < getDistance(b.first, b.second);
  });
  return pairs;
}

std::optional<std::reference_wrapper<Circuit>>
findCircuitContaining(Circuits& circuits, const Junction& junction)
{
  for (auto& circuit : circuits) {
    if (circuit.contains(junction)) {
      return circuit;
    }
  }
  return std::nullopt;
}

template<bool PART_1>
auto
buildCircuits(const Pairs& pairs, std::size_t totalJunctionsOrSteps)
{
  Circuits circuits{};
  std::size_t step = 0;
  std::size_t part2 = 0;

  auto testCondition = [&]() {
    if constexpr (!PART_1) {
      return [&]() {
        const auto size = circuits.size();
        return size != 1 ||
               std::begin(circuits)->size() != totalJunctionsOrSteps;
      };
    } else {
      return [&]() { return step < totalJunctionsOrSteps; };
    }
  }();

  while (testCondition()) {
    const auto& [a, b] = pairs[step++];
    const auto circuitA = findCircuitContaining(circuits, a);
    const auto circuitB = findCircuitContaining(circuits, b);
    part2 = a.x * b.x;
    if (circuitA && !circuitB) {
      circuitA->get().insert(b);
    } else if (!circuitA && circuitB) {
      circuitB->get().insert(a);
    } else if (!circuitA && !circuitB) {
      Circuit newCircuit;
      newCircuit.insert(a);
      newCircuit.insert(b);
      circuits.push_back(std::move(newCircuit));
    } else {
      if (&circuitA->get() != &circuitB->get()) {
        Circuit& targetCircuit = circuitA->get();
        Circuit& sourceCircuit = circuitB->get();
        targetCircuit.insert(sourceCircuit.begin(), sourceCircuit.end());
        circuits.erase(std::begin(std::ranges::remove(circuits, sourceCircuit)),
                       circuits.end());
      }
    }
  }
  if constexpr (PART_1) {
    return circuits;
  } else {
    return part2;
  }
}
}
namespace aoc25::day8 {

Result
solve(const Input& input, std::size_t part1Steps)
{
  const auto junctions = input.size();
  const auto pairs = getClosestPairs(input);
  auto circuits = buildCircuits<true>(pairs, part1Steps);
  std::ranges::sort(circuits, [](const Circuit& a, const Circuit& b) {
    return a.size() > b.size(); // NOLINT
  });

  std::uint64_t part1 = 1;
  for (std::size_t i = 0; i < 3; ++i) {
    part1 *= circuits[i].size();
  }

  const std::uint64_t part2 = buildCircuits<false>(pairs, junctions);
  return Result{ part1, part2 };
}

}
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif