#ifndef AOC_TESTS_PARSING_HPP // NOLINT
#define AOC_TESTS_PARSING_HPP

#include <sstream>
#include <tuple>

namespace inputs {

template<typename... Args>
auto
parseLine(const std::string& line)
{
  std::tuple<Args...> result{};
  std::stringstream isstream{ line };
  [&]<std::size_t... Is>(std::index_sequence<Is...>) {
    ((isstream >> std::get<Is>(result)), ...);
  }(std::index_sequence_for<Args...>());
  return result;
}

};
#endif