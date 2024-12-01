#ifndef AOC_TESTS_PARSING_HPP // NOLINT
#define AOC_TESTS_PARSING_HPP

#include <fstream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>

namespace inputs {

class FileReader
{
public:
  explicit FileReader(std::string_view path) : m_ifs(path.data()) {}
  [[nodiscard]] std::optional<std::string> readLine() {
      if ( std::getline(m_ifs,m_line)) {
        return m_line; 
      }
      return std::nullopt;
  }
private:
  std::ifstream m_ifs;
  std::string m_line;
};

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