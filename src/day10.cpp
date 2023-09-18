#include "day10.hpp"

#include <iostream>

namespace aoc22::day10 {
static constexpr std::size_t firstSignalCycle = 20;
static constexpr std::size_t signalPeriod = CRT_WIDTH;

std::size_t
getCyclesToNextSignalCycle(std::size_t cycles)
{
  if (cycles > firstSignalCycle) {
    auto rem = (cycles - firstSignalCycle) % signalPeriod;
    if (rem != 0) {
      rem = signalPeriod - rem;
    }
    return rem;
  } else {
    return firstSignalCycle - cycles;
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
  Crt crt{};
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

    auto renderPixel = [&crt,&oldRegisterX](const auto cycle){
    const auto column = (cycle - 1) % CRT_WIDTH;
    const auto row = (cycle - 1) / CRT_WIDTH;
    if (static_cast<int64_t>(column) >= oldRegisterX - 1 &&
        static_cast<int64_t>(column) <= oldRegisterX + 1) {
        crt[row][column] = '#';
    } else {
        crt[row][column] = '.';
    }
    };

    renderPixel(oldCycle);
    if (cycle - oldCycle > 1) {
        renderPixel(oldCycle + 1);
    }
  }
  return std::make_pair(resultPart1, crt);
}
}