
module aoc25.day10;

import std;
import aoc.matrix;

namespace {

struct Rational
{
  std::int32_t nom{ 0 };
  std::int32_t denom{ 1 };

  template<typename T>
  explicit Rational(const T& t)
    : nom(static_cast<std::int32_t>(t))
  {
  }

  Rational()
    : nom(0)
    , denom(1) {};

  Rational(std::int32_t nom_, std::int32_t denom_)
    : nom(nom_)
    , denom(denom_)
  {
  }

  explicit operator double() const
  {
    return static_cast<double>(nom) / static_cast<double>(denom);
  }

  friend Rational operator*(const Rational& lhs, const Rational& rhs)
  {
    return Rational{ lhs.nom * rhs.nom, lhs.denom * rhs.denom };
  }

  friend Rational operator-(const Rational& lhs, const Rational& rhs)
  {
    const auto newDenom = lhs.denom * rhs.denom;
    const auto newNom = (lhs.nom * rhs.denom) - (lhs.denom * rhs.nom);
    return { newNom, newDenom };
  }

  friend Rational operator+(const Rational& lhs, const Rational& rhs)
  {
    const auto newDenom = lhs.denom * rhs.denom;
    const auto newNom = (lhs.nom * rhs.denom) + (lhs.denom * rhs.nom);
    return { newNom, newDenom };
  }

  friend Rational operator/(const Rational& lhs, const Rational& rhs)
  {
    return lhs * Rational{ rhs.denom, rhs.nom };
  }

  Rational& operator+=(const Rational& rhs)
  {
    auto result = rhs + *this;
    std::swap(*this, result);
    return *this;
  }

  Rational& operator-=(const Rational& rhs)
  {
    auto result = *this - rhs;
    std::swap(*this, result);
    return *this;
  }
  Rational operator-() const { return { -nom, denom }; }

  [[nodiscard]] bool isInteger() const { return nom % denom == 0; }

  void reduce()
  {
    if ((nom < 0 && denom < 0) || (nom >= 0 && denom < 0)) {
      nom = -nom;
      denom = -denom;
    }
    const auto gcd = std::gcd(nom, denom);
    nom /= gcd;
    denom /= gcd;
  }
  // auto operator<=>(const Rational&) const = default;
};

const auto printRational = [](const Rational r) {
  auto tmp = r;
  tmp.reduce();
  if ((tmp.nom == 0 && tmp.denom != 0) || tmp.denom == 1) {
    return std::format("{: }", tmp.nom);
  }
  return std::format("({: }/{: })", tmp.nom, tmp.denom);
};
const Rational Zero = Rational{ 0, 1 };

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
  std::vector<Rational> coefficients;
  Rational constant = { 0, 1 };

  auto operator()(const std::vector<std::int32_t>& variableValues) const
  {
    Rational result = constant;
    for (std::size_t i = 0; i < coefficients.size(); ++i) {
      result += coefficients[i] * static_cast<Rational>(variableValues[i]);
      result.reduce();
    }
    if (result.denom == 0) {
      std::abort();
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
    if (constant.denom == 0) {
      std::abort();
    }
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

  void print() const
  {
    bool first = true;
    for (std::size_t i = 0; i < coefficients.size(); ++i) {
      if (i > 0 && coefficients[i].nom != 0 && !first) {
        std::print("+ ");
        first = false;
      }
      if (coefficients[i].nom != 0) {
        std::print("{}*X{} ", printRational(coefficients[i]), i);
        first = false;
      }
    }
    if (constant.nom != 0) {
      if (!first) {
        std::print("+ {}", printRational(constant));
      } else {
        std::print("{}", printRational(constant));
      }
    }
    std::println();
  }
};

using Matrix = aoc::matrix::Matrix<std::int32_t>;
using RefMatrix = aoc::matrix::Matrix<Rational>;

void
swapRows(RefMatrix& matrix, std::size_t row1, std::size_t row2)
{
  const auto colCount = matrix.width();
  for (std::size_t k = 0; k < colCount; ++k) {
    std::swap(matrix[k, row1], matrix[k, row2]);
  }
}

auto
gaussianElimination(const Matrix& augmentedMatrix)
{
  const std::size_t rowCount = augmentedMatrix.height();
  const std::size_t colCount = augmentedMatrix.width();

  RefMatrix refMatrix{ { colCount, rowCount }, Zero };
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
  equations.resize(colCount - 1, {});

  for (std::size_t row = rowCount - 1; row < rowCount; --row) {
    const auto targetVar = dependentVariables[row];
    Equation equation{};
    for (std::size_t col = 0; col < colCount - 1; ++col) {
      equation.coefficients.resize(colCount - 1, { 0, 1 });
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


  const auto refMatrix = gaussianElimination(augmentedMatrix);
  const auto& [equations, freeVariables] =
    getEquationsAndFreeVariables(refMatrix);

  const auto freeVariableSize = freeVariables.size();
  std::uint64_t maxFreeVariableSearchRange = 30;
  if (freeVariableSize >= 6) {
    maxFreeVariableSearchRange = 30;
  }
  if (freeVariableSize < 3) {
    maxFreeVariableSearchRange = 200;
  }
  if (freeVariableSize == 3) {
    maxFreeVariableSearchRange = 200;
  }
  if (freeVariableSize == 5) {
    maxFreeVariableSearchRange = 30;
  }
  const auto searchRange = static_cast<std::uint64_t>(
    std::pow(maxFreeVariableSearchRange, freeVariables.size()));

  std::uint64_t minPresses = std::numeric_limits<std::uint64_t>::max();
  std::vector<std::int32_t> variableValues(machine.wirings.size(), 0);

  for (std::uint64_t freeVarCombination = 0; freeVarCombination < searchRange;
       ++freeVarCombination) {
    std::uint64_t remainder = freeVarCombination;
    for (std::size_t i = 0; i < freeVariables.size(); ++i) {
      variableValues[freeVariables[i]] =
        static_cast<std::int32_t>(remainder % maxFreeVariableSearchRange);
      remainder /= maxFreeVariableSearchRange;
    }
    std::uint64_t totalPresses = 0;
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
      totalPresses +=
        std::accumulate(variableValues.begin(),
                        variableValues.end(),
                        0ULL,
                        [](std::uint64_t sum, std::int32_t val) {
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