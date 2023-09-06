#include "day7.hpp"

#include <cassert>
#include <memory>
#include <stdexcept>

namespace aoc22::day7 {

class Filetree
{
  struct Node;
  using Dir = std::vector<std::unique_ptr<Node>>;
  using SizeOrDir = std::variant<std::size_t, Dir>;
  struct Node
  {
    std::string name;
    Node* parent_dir;
    std::variant<std::size_t, Dir> size_or_dir;

    bool is_dir() const noexcept { return size_or_dir.index() == 1; }
    Dir& get_dir() noexcept { return std::get<Dir>(size_or_dir); }
  };

  Node m_root{ .name = std::string{ ROOT_DIR },
               .parent_dir = nullptr,
               .size_or_dir = Dir{} };
  Node* m_cwd{ &m_root };

  static constexpr std::string_view ROOT_DIR = "/";
  static constexpr std::string_view UP_DIR = "..";

public:
  void add_directory(std::string_view dir_name)
  {
    assert(m_cwd->is_dir());
    auto& dir = m_cwd->get_dir();
    const auto it = std::ranges::find_if(
      dir, [&dir_name](const auto& node) { return node->name == dir_name; });
    if (it != dir.end()) {
      throw std::runtime_error("Element already exists!");
    }
    dir.push_back(std::make_unique<Node>(Node{ .name = std::string{ dir_name },
                                               .parent_dir = m_cwd,
                                               .size_or_dir = Dir{} }));
  }
  void add_file(std::string_view file_name, std::size_t size)
  {
    assert(m_cwd->is_dir());
    auto& dir = m_cwd->get_dir();
    const auto it = std::ranges::find_if(
      dir, [&file_name](const auto& node) { return node->name == file_name; });
    if (it != dir.end()) {
      throw std::runtime_error("Element already exists!");
    }
    dir.push_back(std::make_unique<Node>(Node{ .name = std::string{ file_name },
                                               .parent_dir = m_cwd,
                                               .size_or_dir = size }));
  }

  void change_directory(std::string_view dir_name)
  {
    assert(m_cwd->is_dir());
    if (dir_name == ROOT_DIR) {
      m_cwd = &m_root;
    } else if (dir_name == "..") {
      if (m_cwd->parent_dir) {
        m_cwd = m_cwd->parent_dir;
      }
    } else {
      const auto& dir = m_cwd->get_dir();
      const auto it = std::ranges::find_if(dir, [&dir_name](const auto& node) {
        return node->is_dir() && node->name == dir_name;
      });
      if (it == dir.end()) {
        throw std::runtime_error("Directory not found");
      }
      m_cwd = it->get();
    }
  }
};

std::pair<std::size_t, std::size_t>
solve(const Input& input)
{
  Filetree ft{};
  std::size_t i = 0;
  constexpr std::size_t result = 0;
  for (; i < input.size(); ++i) {
    const auto& record = input[i];
    switch (record.index()) {
      case 0: {
        const auto cd = std::get<ChangeDirectory>(record);
        ft.change_directory(cd.directory);
      } break;
        // case 1:
        // break;
      case 2: {
        const auto dir = std::get<Directory>(record);
        ft.add_directory(dir.name);
      } break;
      case 3: {
        const auto file = std::get<File>(record);
        ft.add_file(file.name, file.size);
      } break;
    }
  }

  return std::make_pair(result, result);
}
};
