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

struct Equation
{
  std::vector<std::int32_t> coefficients;
  std::int32_t constant;

  auto operator()(const std::vector<std::int32_t>& variableValues) const
  {
    std::int32_t result = constant;
    for (std::size_t i = 0; i < coefficients.size(); ++i) {
      result += coefficients[i] * variableValues[i];
    }
    return result;
  }
  friend Equation operator*(std::int32_t scalar, const Equation& equation)
  {
    Equation result{};
    result.constant = scalar * equation.constant;
    result.coefficients.resize(equation.coefficients.size());
    for (std::size_t i = 0; i < equation.coefficients.size(); ++i) {
      result.coefficients[i] = scalar * equation.coefficients[i];
    }
    return result;
  }

  friend Equation operator+(const Equation& lhs, const Equation& rhs)
  {
    Equation result{};
    result.constant = lhs.constant + rhs.constant;
    const auto size = std::max(lhs.coefficients.size(), rhs.coefficients.size());
    result.coefficients.resize(size, 0);
    for (std::size_t i = 0; i < size; ++i) {
      std::int32_t lhsCoeff = (i < lhs.coefficients.size()) ? lhs.coefficients[i] : 0;
      std::int32_t rhsCoeff = (i < rhs.coefficients.size()) ? rhs.coefficients[i] : 0;
      result.coefficients[i] = lhsCoeff + rhsCoeff;
    }
    return result;
  }

};

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

auto
gaussianElimination(Matrix& augmentedMatrix)
{
  const auto rowCount = augmentedMatrix.height();
  const auto colCount = augmentedMatrix.width();
  for (std::size_t pivotColumn = 0; pivotColumn < colCount ; ++pivotColumn) {
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
  std::map<std::size_t,std::size_t> dependentVariables{};
  for (std::size_t row = 0; row < rowCount; ++row) {
    bool allZero = true;
    for (std::size_t c = 0; c < colCount - 1; ++c) {
      if (augmentedMatrix[c, row] != 0) {
        allZero = false;
        dependentVariables[row] = c;
        break;
      }
    }
    if (allZero && augmentedMatrix[colCount - 1, row] != 0) {
      throw std::runtime_error("No solution exists");
    }
  }
  auto containsValue = [](const std::map<std::size_t,std::size_t>& map, std::size_t value) {
    for (const auto& [key, val] : map) {
      if (val == value) {
        return true;
      }
    }
    return false;
  };
  std::vector<std::size_t> freeVariables{};
  for (std::size_t i = 0; i < colCount - 1; ++i) {
    if (!containsValue(dependentVariables, i)) {
      freeVariables.push_back(i);
    }
  }
  std::vector<Equation> equations{};
  equations.resize(colCount - 1);

  for (std::size_t row = rowCount - 1; row < rowCount; --row) {
    const auto targetVar = dependentVariables[row];

    for(std::size_t col = 0 ; col < colCount - 1; ++col) {
      if (col != targetVar && augmentedMatrix[col, row] != 0) {
        const auto coefficient = -augmentedMatrix[col, row];
        equations[targetVar].coefficients.resize(colCount - 1, 0);
        equations[targetVar].coefficients[col] = coefficient;
      }
    }

    Equation equation{};

    equations[targetVar] = equation;
  }

  return std::tuple{ equations, freeVariables };
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

  const auto& [equations, freeVariables] = gaussianElimination(augmentedMatrix);
  std::println("Augmented Matrix after elimination:");
  augmentedMatrix.print(std::identity{});
  std::println();
  std::println("Free variables {}", freeVariables);
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