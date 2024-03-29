
#ifndef AOC22_DAY7_HPP
#define AOC22_DAY7_HPP

#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace aoc22::day7 {

struct alignas(32) ChangeDirectory
{
  std::string directory;
};

enum class List
{
};

struct alignas(32) Directory
{
  std::string name;
};

struct File // NOLINT
{
  std::string name;
  std::size_t size;
};

using Record = std::variant<ChangeDirectory, List, Directory, File>;
using Input = std::vector<Record>;

std::pair<std::size_t, std::size_t>
solve(const Input& input);

};

#endif