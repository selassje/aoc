#include "day10.hpp"

namespace aoc22::day10 {
static constexpr std::size_t firstSignalCycle = 20;
static constexpr std::size_t signalPeriod = 40;

std::size_t
getCyclesToNextSignalCycle(std::size_t cycles)
{
  if (cycles >= 20) {
    return (cycles - firstSignalCycle) % signalPeriod;
  } else {
    return 20 - cycles;
  }
};

Result
solve(const Input& input)
{
  const auto inputSize = input.size();

  std::int64_t registerX{ 1 };
  std::size_t cycle = 1;

  static constexpr std::size_t noopCycles = 1;
  static constexpr std::size_t addXCycles = 2;

  std::int64_t resultPart1 = 0;
  for (std::size_t i = 0; i < inputSize; ++i) {
    const auto cyclesToNextSignalCycle = getCyclesToNextSignalCycle(cycle);
    const auto oldRegisterX = registerX;
    const auto oldCycle = cycle;

    const auto& instruction = input[i];
    switch (instruction.index()) {
      case 0:
        cycle += noopCycles;
        break;
      case 1:
        const auto& addX = std::get<AddX>(instruction);
        cycle += addXCycles;
        registerX += addX.x;
        break;
    }

    if (cyclesToNextSignalCycle == 0) {
      resultPart1 += static_cast<std::int64_t>(oldCycle) * oldRegisterX;
    } else if (cyclesToNextSignalCycle < (cycle - oldCycle)) {
      resultPart1 +=
        static_cast<std::int64_t>(cyclesToNextSignalCycle + oldCycle) *
        oldRegisterX;
    }
  }

  if(getCyclesToNextSignalCycle(cycle) == 0)
  {
      resultPart1 += static_cast<std::int64_t>(cycle) * registerX;
  }

  return std::make_pair(resultPart1, resultPart1);
}
}