#include "day10.hpp"

#include <cstddef>
#include <cstdint>
#include <utility>

namespace aoc22::day10 {
static constexpr std::size_t FIRST_SIGNAL_CYCLE = 20;
static constexpr std::size_t SIGNAL_PERIOD = CRT_WIDTH;

std::size_t
getCyclesToNextSignalCycle(std::size_t cycles)
{
  if (cycles > FIRST_SIGNAL_CYCLE) {
    auto rem = (cycles - FIRST_SIGNAL_CYCLE) % SIGNAL_PERIOD;
    if (rem != 0) {
      rem = SIGNAL_PERIOD - rem;
    }
    return rem;
  }
  return FIRST_SIGNAL_CYCLE - cycles;
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
    std::size_t addedCycles = noopCycles;
    std::int64_t addedX = 0;

    if (input[i].index() == 0) {
      const auto& addX = std::get<AddX>(input[i]);
      addedCycles = addXCycles;
      addedX = addX.x;
    }

    if (cyclesToNextSignalCycle == 0) {
      resultPart1 += static_cast<std::int64_t>(cycle) * registerX;
    } else if (cyclesToNextSignalCycle < addedCycles) {
      resultPart1 +=
        static_cast<std::int64_t>(cyclesToNextSignalCycle + cycle) * registerX;
    }
    auto renderPixel = [&crt, &registerX](const auto cycle_) {
      const auto column = (cycle_ - 1) % CRT_WIDTH;
      const auto row = (cycle_ - 1) / CRT_WIDTH;
      const auto columnSigned = static_cast<int64_t>(column);
      crt[row][column] =
        columnSigned >= registerX - 1 && columnSigned <= registerX + 1 ? '#'
                                                                       : '.';
    };

    renderPixel(cycle);
    if (addedCycles > 1) {
      renderPixel(cycle + 1);
    }

    registerX += addedX;
    cycle += addedCycles;
  }
  return std::make_pair(resultPart1, crt);
}
}