#include "day13.hpp"

#include <algorithm>
#include <memory>
#include <ranges>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace aoc22::day13 {

struct List;

using Element = std::variant<std::unique_ptr<List>, std::size_t>;

struct List
{
  std::vector<Element> elements;
};

Element
parse(std::string_view tokens)
{
  if (tokens[0] == '[') {
    auto listPtr = std::make_unique<List>();

    std::size_t nextElementStart = 0;
    std::size_t level = 0;

    for (std::size_t i = 1; i < tokens.size(); ++i) {
      if ((tokens[i] == ',' || i == tokens.size() - 1) && level == 0) {
        auto elementStr =
          tokens.substr(nextElementStart + 1, i - nextElementStart);
        listPtr->elements.push_back(parse(elementStr));
        nextElementStart = i;
      } else if (tokens[i] == '[') {
        ++level;
      } else if (tokens[i] == ']') {
        --level;
      }
    }
    return listPtr;
  } else {
    const auto integerEnd = std::ranges::find_if(
      tokens, [](char token) { return token == ',' || token == ']'; });
    std::string strInteger{ tokens.begin(), integerEnd };
    return static_cast<std::size_t>(
      std::strtoull(strInteger.c_str(), nullptr, 10));
  }
}

enum class CmprResult {
  Correct,
  Incorrect,
  None,
};

using enum CmprResult;

CmprResult compare(const Element &left, const Element& right) {
  CmprResult result = None;
    if ( left.index() == 1 && right.index() == 1) {
      const auto leftInteger = std::get<1>(left);
      const auto rightInteger = std::get<1>(right);
      if (leftInteger <  rightInteger ) {
        result = Correct;
      } else if (leftInteger > rightInteger) {
        result = Incorrect;
      }
    } else if ( left.index() == 0 && right.index() == 0) {
        const auto& leftList = std::get<0>(left);
        const auto& rightList = std::get<0>(right);
        

    }
  return result;
}

Result
solve(const Input& input)
{
  for (const auto& [first, second] : input) {
    auto firstParsed = parse(first);
    auto secondParsed = parse(second);
  }

  return { 0, 0 };
}

}