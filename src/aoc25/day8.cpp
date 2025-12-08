module aoc25.day8;

import std;

namespace {

using aoc25::day8::Input;
using aoc25::day8::Junction;
using Pair = std::pair<Junction, Junction>;
using Pairs = std::vector<Pair>;
using Circuit = std::flat_set<Junction>;
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

template<std::size_t STEPS>
auto
buildCircuits(const Pairs& pairs)
{
  Circuits circuits{};
  for (std::size_t step = 0; step < STEPS; ++step) {
    const auto& [a, b] = pairs[step];
    const auto circuitA = findCircuitContaining(circuits, a);
    const auto circuitB = findCircuitContaining(circuits, b);
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
      if (circuitA != circuitB) {
        Circuit& targetCircuit = circuitA->get();
        Circuit& sourceCircuit = circuitB->get();
        targetCircuit.insert(sourceCircuit.begin(), sourceCircuit.end());
        circuits.erase(
          std::remove(circuits.begin(), circuits.end(), sourceCircuit),
          circuits.end());
      }
    }
  }
  return circuits;
}
}
namespace aoc25::day8 {

Result
solve(const Input& input)
{
  const auto pairs = getClosestPairs(input);
  auto circuits = buildCircuits<1000>(pairs);
  std::ranges::sort(circuits, [](const Circuit& a, const Circuit& b) {
    return a.size() > b.size();
  });

  std::uint64_t part1 = 1;
  for (std::size_t i = 0; i < 3; ++i) {
    std::print("Circuits size: {}\n", circuits[i].size());
    part1 *= circuits[i].size();
  }

  std::uint64_t part2 = part1;
  return Result{ part1, part2 };
}
}
