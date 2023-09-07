#include "day7.hpp"

#include <cassert>
#include <memory>
#include <ranges>
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
    Dir& get_dir() const noexcept { return const_cast<Dir&>(std::get<Dir>(size_or_dir)); }
  };

  Node m_root{ .name = std::string{ ROOT_DIR },
               .parent_dir = nullptr,
               .size_or_dir = Dir{} };
  Node* m_cwd{ &m_root };

  static constexpr std::string_view ROOT_DIR = "/";
  static constexpr std::string_view UP_DIR = "..";

public:
template<typename T> 
  void add_item(const T &item)
  {
    assert(m_cwd->is_dir());
    auto& dir = m_cwd->get_dir();
    const auto it = std::ranges::find_if(
      dir, [item](const auto& node) { return node->name == item.name; });
    if (it != dir.end()) {
      throw std::runtime_error("Element already exists!");
    }

    std::variant<std::size_t, Dir> size_or_dir = Dir{};
    if constexpr(std::same_as<File,T>)
    {
        size_or_dir = item.size;
    }
    dir.push_back(std::make_unique<Node>(Node{ .name = std::string{ item.name },
                                               .parent_dir = m_cwd,
                                               .size_or_dir = std::move(size_or_dir)}));
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

  std::size_t get_size() const {
    return get_size(m_root);
  }
  std::size_t get_total_dir_size() const {
    return get_total_dir_size(m_root);
  }
  
private:
  static std::size_t get_size(const Node &node) {
      if (node.is_dir())
      {
          std::size_t total = 0;
          for (const auto& child : node.get_dir() )
          {
            total += get_size(*child);
          }
          return total;
      }
      return std::get<std::size_t>(node.size_or_dir);
  }
  
  static std::size_t get_total_dir_size(const Node &node) {
      const std::size_t max_size = 100000;
      if (node.is_dir())
      {
          std::size_t total = get_size(node);
          if ( total > max_size)
          {
            total = 0;
          }
          for (const auto& child : node.get_dir() )
          {
              total += get_total_dir_size(*child);
          }
          return total;
      }
      return 0;
  }

};

std::pair<std::size_t, std::size_t>
solve(const Input& input)
{
  Filetree ft{};
  std::size_t i = 0;
  for (; i < input.size(); ++i) {
    const auto& record = input[i];
    switch (record.index()) {
      case 0: {
        const auto cd = std::get<ChangeDirectory>(record);
        ft.change_directory(cd.directory);
      } break;
      case 2: {
        const auto dir = std::get<Directory>(record);
        ft.add_item(dir);
      } break;
      case 3: {
        const auto file = std::get<File>(record);
        ft.add_item(file);
      } break;
    }
  }
  const std::size_t result = ft.get_total_dir_size();;
  return std::make_pair(result, result);
}
};
