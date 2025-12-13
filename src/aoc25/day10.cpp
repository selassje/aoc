
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
  std::vector<double> coefficients;
  double constant = 0;
  std::uint32_t parity = 1;

  auto operator()(const std::vector<std::int32_t>& variableValues) const
  {
    double result = constant;
    for (std::size_t i = 0; i < coefficients.size(); ++i) {
      result += coefficients[i] * variableValues[i];
    }
    return static_cast<std::int32_t>(std::round(result));
  }
  friend Equation operator*(double scalar, const Equation& equation)
  {
    Equation result{};
    result.constant = scalar * equation.constant;
    result.coefficients.resize(equation.coefficients.size());
    for (std::size_t i = 0; i < equation.coefficients.size(); ++i) {
      result.coefficients[i] = scalar * equation.coefficients[i];
    }
    return result;
  }

  Equation& substitute(std::size_t variableIndex, const Equation& substitution)
  {
    const auto coeff = coefficients[variableIndex];
    constant += coeff * substitution.constant;
    for (std::size_t i = 0; i < substitution.coefficients.size(); ++i) {
      if (i != variableIndex) {
        if (i >= coefficients.size()) {
          coefficients.resize(i + 1, 0);
        }
        coefficients[i] += coeff * substitution.coefficients[i];
      }
    }
    coefficients[variableIndex] = 0;
    return *this;
  }

  void print() const
  {
    bool first = true;
    for (std::size_t i = 0; i < coefficients.size(); ++i) {
      if (i > 0 && coefficients[i] != 0 && !first) {
        std::print("+ ");
        first = false;
      }
      if (coefficients[i] != 0) {
        std::print("{}*X{} ", coefficients[i], i);
        first = false;
      }
    }
    if (constant != 0) {
      if (!first) {
        std::print("+ {}", constant);
      } else {
        std::print("{}", constant);
      }
    }
    std::println();
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
  for (std::size_t pivotColumn = 0; pivotColumn < colCount - 1; ++pivotColumn) {
    std::size_t pivotRow = pivotColumn;
    std::size_t row = pivotRow;
    while (row < rowCount && augmentedMatrix[pivotColumn, row] == 0) {
      ++row;
    }
    pivotRow = row;
    if (pivotRow == rowCount) {
      continue;
    }
    std::size_t currentPivotRow = pivotColumn; // row where we want pivot
    if (pivotRow != currentPivotRow)
      swapRows(augmentedMatrix, currentPivotRow, pivotRow);

    for (std::size_t r = currentPivotRow + 1; r < rowCount; ++r) {
      if (augmentedMatrix[pivotColumn, r] != 0) {
        const auto multiple = augmentedMatrix[pivotColumn, r] /
                              augmentedMatrix[pivotColumn, currentPivotRow];
        substractRow(augmentedMatrix, r, currentPivotRow, multiple);
      }
    }
  }
  std::map<std::size_t, std::size_t> dependentVariables{};
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
  auto containsValue = [](const std::map<std::size_t, std::size_t>& map,
                          std::size_t value) {
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
    Equation equation{};
    for (std::size_t col = 0; col < colCount - 1; ++col) {
      equation.coefficients.resize(colCount - 1, 0);
      if (col != targetVar && augmentedMatrix[col, row] != 0) {
        const auto coefficient = augmentedMatrix[col, row];
        equation.coefficients[col] = -coefficient;
      }
    }
    equation.constant = augmentedMatrix[colCount - 1, row];
    const double coeff = augmentedMatrix[targetVar, row];
    if (false) {
      if (coeff  <  0)  {
      equation = -1* equation;
      }
      equation.parity =
        static_cast<std::uint32_t>(std::round(std::abs(coeff)));
    } else {
      const double coeffInv = 1. / coeff;
      equation = coeffInv * equation;
      equation.parity = 1;
    }

    auto orgEq = equation;
    std::size_t prevRow = row + 1;
    while (prevRow < rowCount) {
      const auto varIndex = dependentVariables[prevRow];
      if (equation.coefficients[varIndex] != 0) {
        equation.substitute(varIndex, equations[varIndex]);
      }
      ++prevRow;
    }
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
  std::println("Equations:");
  for (std::size_t i = 0; i < equations.size(); ++i) {
    if (equations[i].parity != 1) {
      std::print("{}", equations[i].parity);
    }
    std::print("X{} = ", i);
    equations[i].print();
  }

  const auto freeVariableSize = freeVariables.size();
  std::uint64_t maxFreeVariableSearchRange = 25;
  if (freeVariableSize >= 6) {
    maxFreeVariableSearchRange = 10;
  }
  if (freeVariableSize <= 3) {
    maxFreeVariableSearchRange = 180;
  }
  std::println("MaxSearch space per variable {}", maxFreeVariableSearchRange);
  const auto searchRange = static_cast<std::uint64_t>(
    std::pow(maxFreeVariableSearchRange, freeVariables.size()));

  std::uint64_t minPresses = std::numeric_limits<std::uint64_t>::max();
  std::vector<std::int32_t> variableValues(machine.wirings.size(), 0);
  std::vector<std::int32_t> minVariableValues(machine.wirings.size(), 0);

  for (std::uint64_t freeVarCombination = 0; freeVarCombination < searchRange;
       ++freeVarCombination) {
    // variableValues.clear();
    std::uint64_t remainder = freeVarCombination;
    for (std::size_t i = 0; i < freeVariables.size(); ++i) {
      variableValues[freeVariables[i]] =
        static_cast<std::int32_t>(remainder % maxFreeVariableSearchRange);
      remainder /= maxFreeVariableSearchRange;
      const auto parity = equations[freeVariables[i]].parity;
      if (parity == 0) {
        std::println("Parity 0");
        std::abort();
      }
      // variableValues[freeVariables[i]] *=
      //   static_cast<std::int32_t>(equations[freeVariables[i]].parity);
    }
    std::uint64_t totalPresses = 0;
    bool validSolution = true;
    for (std::size_t i = 0; i < equations.size(); ++i) {
      const auto value = equations[i](variableValues);
      if (value < 0) {
        validSolution = false;
        break;
      }
      if (std::ranges::find(freeVariables, i) == freeVariables.end()) {
        variableValues[i] = value;
        const auto parity = static_cast<std::int32_t>(equations[i].parity);
        if (parity == 2) {
          std::println("Parity 0");
          // std::abort();
        }
        if (variableValues[i] % parity != 0) {
          validSolution = false;
        }

        variableValues[i] /= static_cast<std::int32_t>(parity);
      }
    }
    if (validSolution) {
      totalPresses +=
        std::accumulate(variableValues.begin(),
                        variableValues.end(),
                        0ULL,
                        [](std::uint64_t sum, std::int32_t val) {
                          return sum + static_cast<std::uint64_t>(val);
                        });
      if (totalPresses < minPresses) {
        minVariableValues = variableValues;
      }
      minPresses = std::min(totalPresses, minPresses);
    }
  }

  std::println("Min presses {}", minPresses);
  std::println("Solution {}", minVariableValues);
  for (std::size_t i = 0; i < minVariableValues.size(); ++i) {
    if (std::ranges::find(freeVariables, i) == freeVariables.end()) {
      minVariableValues[i] = 0;
    }
  }
  std::println("Free variables for solution {}", minVariableValues);
  if (minPresses == std::numeric_limits<std::uint64_t>::max()) {
    //  std::abort();
  }

  return minPresses;
}
}

namespace aoc25::day10 {

Result
solve(const Input& input)
{
  std::uint64_t part1 = 0;
  std::uint64_t part2 = 0;
  std::size_t i = 0;
  std::size_t infCount = 0;
  for (const auto& machine : input) {
    part1 += countMinimumPressesForLights(machine);
    std::println("Solving machine {}", i++);
    const auto minPressPart2 = countMinimumPressesForJoltages(machine);
    if (minPressPart2 == std::numeric_limits<std::uint64_t>::max()) {
      ++infCount;
      std::println("Could not find solutions for {}",i);
     // std::abort();
    } else {
      part2 += minPressPart2;
    }

    if (i == 10) {
      // std::abort();
    }
  }
  std::println("Wrong solutions {}", infCount);
  return { part1, part2 };
}
}