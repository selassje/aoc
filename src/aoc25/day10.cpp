module aoc25.day10;

import std;
import aoc.graph;
import aoc.matrix;
import aoc.rational;

using Rational = aoc::rational::Rational;
using Shape = aoc::matrix::Matrix<std::int32_t>;
using RefMatrix = aoc::matrix::Matrix<Rational>;
namespace {

using namespace aoc25::day10; // NOLINT

std::uint64_t
countMinimumPressesForLights(const Machine& machine)
{
  const std::uint64_t statesCount = 1ULL << machine.lights.size();
  auto states = [=] {
    return std::views::iota(static_cast<std::uint64_t>(0), statesCount);
  };

  aoc::graph::Graph<std::uint64_t, std::uint64_t> graph(states());
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

  auto toggleLight = [](std::uint64_t state, std::size_t lightIndex) {
    return state ^ (1ULL << lightIndex);
  };
  for (const auto& currentState : states()) {
    for (const auto& wiring : machine.wirings) {
      auto nextState = currentState;
      for (auto lightIndex : wiring) {
        nextState =
          toggleLight(nextState, static_cast<std::size_t>(lightIndex));
      }
      graph[currentState, nextState] = 1;
    }
  }
  return graph.findShortestDistance({ startState, targetState });
}

struct Equation
{
  std::vector<Rational> coefficients;
  Rational constant = { 0, 1 };

  auto operator()(const std::vector<std::int32_t>& variableValues) const
  {
    Rational result = constant;
    for (std::size_t i = 0; i < coefficients.size(); ++i) {
      result += coefficients[i] * static_cast<Rational>(variableValues[i]);
      result.reduce();
    }
    return result;
  }
  friend Equation operator*(Rational scalar, const Equation& equation)
  {
    Equation result{};
    result.constant = scalar * equation.constant;
    result.coefficients.resize(equation.coefficients.size(), { 0, 1 });
    for (std::size_t i = 0; i < equation.coefficients.size(); ++i) {
      result.coefficients[i] = scalar * equation.coefficients[i];
    }
    return result;
  }

  Equation& substitute(std::size_t variableIndex, const Equation& substitution)
  {
    const auto coeff = coefficients[variableIndex];
    constant += coeff * substitution.constant;
    constant.reduce();
    for (std::size_t i = 0; i < substitution.coefficients.size(); ++i) {
      if (i != variableIndex) {
        if (i >= coefficients.size()) {
          coefficients.resize(i + 1, { 0, 1 });
        }
        coefficients[i] += coeff * substitution.coefficients[i];
        coefficients[i].reduce();
      }
    }
    coefficients[variableIndex] = { 0, 1 };
    return *this;
  }
};

void
swapRows(RefMatrix& matrix, std::size_t row1, std::size_t row2)
{
  const auto colCount = matrix.width();
  for (std::size_t k = 0; k < colCount; ++k) {
    std::swap(matrix[k, row1], matrix[k, row2]);
  }
}

auto
gaussianElimination(const Shape& augmentedMatrix)
{
  const std::size_t rowCount = augmentedMatrix.height();
  const std::size_t colCount = augmentedMatrix.width();

  RefMatrix refMatrix{ { colCount, rowCount }, {} };
  for (std::size_t x = 0; x < colCount; ++x) {
    for (std::size_t y = 0; y < rowCount; ++y) {
      refMatrix[x, y] = static_cast<Rational>(augmentedMatrix[x, y]);
    }
  }

  std::size_t pivotRow = 0;
  for (std::size_t pivotCol = 0; pivotCol < colCount - 1 && pivotRow < rowCount;
       ++pivotCol) {
    std::size_t row = pivotRow;
    while (row < rowCount && refMatrix[pivotCol, row].nom == 0) {
      ++row;
    }

    if (row == rowCount) {
      continue;
    }

    if (row != pivotRow) {
      swapRows(refMatrix, row, pivotRow);
    }

    const auto pivot = refMatrix[pivotCol, pivotRow];
    for (std::size_t r = pivotRow + 1; r < rowCount; ++r) {
      if (refMatrix[pivotCol, r].nom == 0) {
        continue;
      }
      const auto factor = refMatrix[pivotCol, r] / pivot;
      for (std::size_t c = 0; c < colCount; ++c) {
        refMatrix[c, r] -= factor * refMatrix[c, pivotRow];
        refMatrix[c, r].reduce();
      }
    }
    ++pivotRow;
  }
  return refMatrix;
}

auto
getEquationsAndFreeVariables(const RefMatrix& refMatrix)
{
  const auto rowCount = refMatrix.height();
  const auto colCount = refMatrix.width();
  std::map<std::size_t, std::size_t> dependentVariables{};
  for (std::size_t row = 0; row < rowCount; ++row) {
    for (std::size_t c = 0; c < colCount - 1; ++c) {
      if (refMatrix[c, row].nom != 0) {
        dependentVariables[row] = c;
        break;
      }
    }
  }
  auto containsValue = [](const std::map<std::size_t, std::size_t>& map,
                          std::size_t value) {
    return std::ranges::any_of(
      map, [&](const auto& pair) { return pair.second == value; });
  };
  std::vector<std::size_t> freeVariables{};
  for (std::size_t i = 0; i < colCount - 1; ++i) {
    if (!containsValue(dependentVariables, i)) {
      freeVariables.push_back(i);
    }
  }
  std::vector<Equation> equations(colCount - 1);
#pragma warning(disable : 6293)
  for (std::size_t row = rowCount - 1; row < rowCount; --row) {
#pragma warning(default : 6293)
    const auto targetVar = dependentVariables[row];
    Equation equation{};
    for (std::size_t col = 0; col < colCount - 1; ++col) {
      equation.coefficients.resize(colCount - 1, {});
      if (col != targetVar && refMatrix[col, row].nom != 0) {
        const auto coefficient = refMatrix[col, row];
        equation.coefficients[col] = -coefficient;
      }
    }
    equation.constant = refMatrix[colCount - 1, row];
    const auto coeff = refMatrix[targetVar, row];
    if (coeff.nom != 0) {
      const auto coeffInv = static_cast<Rational>(1) / coeff;
      equation = coeffInv * equation;
    }

    std::size_t prevRow = row + 1;
    while (prevRow < rowCount) {
      if (dependentVariables.contains(prevRow)) {
        const auto varIndex = dependentVariables[prevRow];
        if (equation.coefficients[varIndex].nom != 0) {
          equation.substitute(varIndex, equations[varIndex]);
        }
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
  Shape augmentedMatrix{ { m + 1, n }, 0 };

  for (std::size_t i = 0; i < n; ++i) {
    augmentedMatrix[m, i] = static_cast<std::int32_t>(machine.joltages[i]);
  }

  for (std::size_t i = 0; i < machine.wirings.size(); ++i) {
    const auto& wiring = machine.wirings[i];
    for (const auto& button : wiring) {
      augmentedMatrix[i, button] = 1;
    }
  }

  const auto refMatrix = gaussianElimination(augmentedMatrix);
  const auto& [equations, freeVariables] =
    getEquationsAndFreeVariables(refMatrix);

  const auto freeVariableSize = freeVariables.size();
  const std::uint64_t maxFreeVariableSearchRange =
    freeVariableSize <= 3 ? 200 : 20;

  const auto searchRange = static_cast<std::uint64_t>(
    std::pow(maxFreeVariableSearchRange, freeVariables.size()));

  std::uint64_t minPresses = std::numeric_limits<std::uint64_t>::max();
  std::vector<std::int32_t> variableValues(machine.wirings.size(), 0);

  for (std::uint64_t freeVarCombination = 0; freeVarCombination < searchRange;
       ++freeVarCombination) {
    std::uint64_t remainder = freeVarCombination;
    for (auto i : freeVariables) {
      variableValues[i] =
        static_cast<std::int32_t>(remainder % maxFreeVariableSearchRange);
      remainder /= maxFreeVariableSearchRange;
    }
    bool validSolution = true;
    for (std::size_t i = 0; i < equations.size(); ++i) {
      const auto value = equations[i](variableValues);
      if (value.nom * value.denom < 0) {
        validSolution = false;
        break;
      }
      if (std::ranges::find(freeVariables, i) == freeVariables.end()) {
        if (!value.isInteger()) {
          validSolution = false;
          break;
        }
        variableValues[i] = value.nom / value.denom;
      }
    }
    if (validSolution) {
      const auto totalPresses = std::ranges::fold_left(
        variableValues, 0ULL, [](std::uint64_t sum, std::int32_t val) {
          return sum + static_cast<std::uint64_t>(val);
        });
      minPresses = std::min(totalPresses, minPresses);
    }
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
  for (const auto& machine : input) {
    part1 += countMinimumPressesForLights(machine);
    part2 += countMinimumPressesForJoltages(machine);
  }
  return { part1, part2 };
}
}