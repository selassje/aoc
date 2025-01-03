#ifndef AOC_TESTS_PARSING_HPP // NOLINT
#define AOC_TESTS_PARSING_HPP

#include <fstream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace inputs {

class FileReader
{
public:
  explicit FileReader(std::string_view path)
    : m_ifs(path.data())
  {
  }
  [[nodiscard]] std::optional<std::string> readLine()
  {
    if (std::getline(m_ifs, m_line)) {
      return m_line;
    }
    return std::nullopt;
  }

  [[nodiscard]] std::string read()
  {
    std::stringstream sstream{};
    sstream << m_ifs.rdbuf();
    return sstream.str();
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

template<typename T>
auto
parseStringDynamic(const std::string& line)
{
  std::vector<T> result{};
  T value{};
  std::stringstream isstream{ line };
  while (isstream >> value) {
    result.push_back(value);
  }
  return result;
}

};
#endif