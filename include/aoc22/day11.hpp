
#ifndef AOC22_DAY11_HPP
#define AOC22_DAY11_HPP

#include <array>
#include <cstdint>
#include <utility>
#include <variant>
#include <vector>

namespace aoc22::day11 {

enum class OperationType
{
  Add,
  Multiply
};

struct Literal
{
  std::uint64_t value;
};

struct Old
{};

using Operand = std::variant<Literal, Old>;

struct alignas(64) Operation
{
  OperationType type;
  Operand op1;
  Operand op2;
};

struct alignas(128) Monkey
{
  Operation operation;
  std::uint64_t divisionTest;
  std::size_t nextMonkeyTestPass;
  std::size_t nextMonkeyTestFail;
  std::vector<std::uint64_t> worryLevels;
};

using Input = std::vector<Monkey>;

using Result = std::pair<std::uint64_t, std::uint64_t>;

Result
solve(const Input& input);
};

#endif