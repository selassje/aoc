module aoc25.day10;

import std;
import aoc.matrix;

namespace {

using namespace aoc25::day10;

std::uint64_t
countMinimumPressesForLights(const Machine& machine)
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
using Matrix = aoc::matrix::Matrix<std::int32_t>;

void
substractRow(Matrix& matrix,
             std::size_t targetRow,
             std::size_t sourceRow,
             std::int32_t multiple)
{
  const auto colCount = matrix.width();
  for (std::size_t k = 0; k < colCount; ++k) {
    matrix[k, targetRow] -= multiple * matrix[k, sourceRow];
  }
}

void
swapRows(Matrix& matrix, std::size_t row1, std::size_t row2)
{
  const auto colCount = matrix.width();
  for (std::size_t k = 0; k < colCount; ++k) {
    std::swap(matrix[k, row1], matrix[k, row2]);
  }
}

void
gaussianElimination(Matrix& augmentedMatrix)
{
  const auto rowCount = augmentedMatrix.height();
  const auto colCount = augmentedMatrix.width();
  for (std::size_t pivotColumn = 0; pivotColumn < colCount; ++pivotColumn) {
    std::size_t pivotRow = pivotColumn;
    while (pivotRow < rowCount && augmentedMatrix[pivotColumn, pivotRow] == 0) {
      ++pivotRow;
    }
    if (pivotRow == rowCount) {
      continue;
    }
    if (pivotRow != pivotColumn) {
      swapRows(augmentedMatrix, pivotColumn, pivotRow);
    }
    for (std::size_t r = pivotColumn + 1; r < rowCount; ++r) {
      if (augmentedMatrix[pivotColumn, r] != 0) {
        const auto multiple = augmentedMatrix[pivotColumn, r] /
                              augmentedMatrix[pivotColumn, pivotColumn];
        substractRow(augmentedMatrix, r, pivotColumn, multiple);
      }
    }
  }
}

std::uint64_t
countMinimumPressesForJoltages(const Machine& machine)
{
  const auto n = machine.joltages.size();
  const auto m = machine.wirings.size();
  Matrix augmentedMatrix{ { m + 1, n }, 0 };

  for (std::size_t i = 0; i < n; ++i) {
    augmentedMatrix[m, i] = static_cast<std::int32_t>(machine.joltages[i]);
  }

  for (std::size_t i = 0; i < machine.wirings.size(); ++i) {
    const auto& wiring = machine.wirings[i];
    for (const auto& button : wiring) {
      augmentedMatrix[i, button] = 1;
    }
  }
  
  std::println("Augmented Matrix before elimination:");
  augmentedMatrix.print(std::identity{});
  
  gaussianElimination(augmentedMatrix);
  std::println("Augmented Matrix after elimination:");
  augmentedMatrix.print(std::identity{});
  std::println();
  return 0;
}
}

namespace aoc25::day10 {

Result
solve(const Input& input)
{
  std::uint64_t part1 = 0;
  std::uint64_t part2 = 0;
  for (const auto& machine : input) {
    part1 += countMinimumPressesForLights(machine);
    part2 += countMinimumPressesForJoltages(machine);
  }
  return { part1, part2 };
}
}