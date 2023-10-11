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

using Element = std::variant<std::shared_ptr<List>, std::size_t>;

struct List
{
  std::vector<Element> elements;
};

Element
parse(std::string_view tokens)
{
  if (tokens[0] == '[') {
    auto listPtr = std::make_shared<List>();

    std::size_t nextElementStart = 0;
    std::size_t level = 0;

    for (std::size_t i = 1; i < tokens.size(); ++i) {
      if ((tokens[i] == ',' || i == tokens.size() - 1) && level == 0) {
        auto elementStr =
          tokens.substr(nextElementStart + 1, i - nextElementStart - 1);
        if (!elementStr.empty()) {
          listPtr->elements.push_back(parse(elementStr));
        }
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

enum class CmprResult
{
  Lesser,
  Greater,
  Equal,
};

using enum CmprResult;

CmprResult
compare(const Element& left, const Element& right)
{
  CmprResult result = Equal;
  if (left.index() == 1 && right.index() == 1) {
    const auto leftInteger = std::get<1>(left);
    const auto rightInteger = std::get<1>(right);
    if (leftInteger < rightInteger) {
      result = Lesser;
    } else if (leftInteger > rightInteger) {
      result = Greater;
    }
  } else if (left.index() == 0 && right.index() == 0) {
    const auto& leftList = std::get<0>(left);
    const auto& rightList = std::get<0>(right);
    const auto minSize =
      std::min(leftList->elements.size(), rightList->elements.size());
    for (std::size_t i = 0; i < minSize; ++i) {
      const auto cmprResult =
        compare(leftList->elements[i], rightList->elements[i]);
      if (cmprResult != Equal) {
        result = cmprResult;
        break;
      }
    }
    if (result == Equal) {
      if (leftList->elements.size() < rightList->elements.size()) {
        result = Lesser;
      } else if (leftList->elements.size() > rightList->elements.size()) {
        result = Greater;
      }
    }
  } else if (left.index() == 1) {
    auto listPtr = std::make_shared<List>();
    listPtr->elements.emplace_back(std::get<1>(left));
    result = compare(std::move(listPtr), right);
  } else {
    auto listPtr = std::make_shared<List>();
    listPtr->elements.emplace_back(std::get<1>(right));
    result = compare(left, std::move(listPtr));
  }
  return result;
}

Result
solve(const Input& input)
{
  const auto divider1 = parse("[[2]]");
  const auto divider2 = parse("[[6]]");

  std::size_t resultPart1 = 0;
  std::vector elements{ divider1, divider2 };
  for (std::size_t i = 0; i < input.size(); ++i) {
    const auto& [first, second] = input[i];
    auto firstParsed = parse(first);
    auto secondParsed = parse(second);
    if (compare(firstParsed, secondParsed) == Lesser) {
      resultPart1 += i + 1;
    }
    elements.emplace_back(firstParsed);
    elements.emplace_back(secondParsed);
  }

  std::ranges::sort(elements, [](const auto& left, const auto& right) {
    return compare(left, right) == Lesser;
  });

  auto divider1Index_ =
    std::ranges::find_if(elements,
                         [&divider1](const auto& packet) {
                           return compare(packet, divider1) == Equal;
                         }) -
    elements.begin();

  auto divider2Index_ =
    std::ranges::find_if(elements,
                         [&divider2](const auto& packet) {
                           return compare(packet, divider2) == Equal;
                         }) -
    elements.begin();

  const auto divider1Index = static_cast<std::size_t>(divider1Index_);
  const auto divider2Index = static_cast<std::size_t>(divider2Index_);

  const auto resultPart2 = (divider1Index + 1) * (divider2Index + 1);

  return { resultPart1, resultPart2 };
}

}