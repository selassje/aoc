#include "day7.hpp"

#include <algorithm>
#include <cassert>
#include <memory>
#include <numeric>
#include <ranges>
#include <stdexcept>

namespace aoc22::day7 {

class FileTree
{
public:
  template<typename T>
  void add_item(const T& item);

  void change_directory(std::string_view dir_name);

  [[nodiscard]] std::vector<std::size_t> get_dir_sizes() const;

private:
  struct Node;
  using Dir = std::vector<std::shared_ptr<Node>>;
  using SizeOrDir = std::variant<std::size_t, Dir>;
  struct Node
  {
    std::string name;
    std::weak_ptr<Node> parent_dir;
    std::variant<std::size_t, Dir> size_or_dir;

    [[nodiscard]] bool is_dir() const noexcept
    {
      return size_or_dir.index() == 1;
    }
    [[nodiscard]] const Dir& get_dir() const
    {
      return std::get<Dir>(size_or_dir); 
    }
    [[nodiscard]] Dir& get_dir() 
    {
      return std::get<Dir>(size_or_dir);
    }
  };

  std::shared_ptr<Node> m_root =
    std::make_shared<Node>(Node{ .name = std::string{ ROOT_DIR },
                                 .parent_dir = {},
                                 .size_or_dir = Dir{} });
  std::shared_ptr<Node> m_cwd{ m_root };

  static constexpr std::string_view ROOT_DIR = "/";
  static constexpr std::string_view UP_DIR = "..";

  static std::size_t get_size(const Node& node);
  static void get_dir_sizes(const Node& node, std::vector<std::size_t>& sizes);
};

template<typename T>
void
FileTree::add_item(const T& item)
{
  assert(m_cwd->is_dir());
  auto& dir = m_cwd->get_dir();
  const auto it = std::ranges::find_if(
    dir, [item](const auto& node) { return node->name == item.name; });
  if (it != dir.end()) {
    throw std::runtime_error("Element already exists!");
  }

  std::variant<std::size_t, Dir> size_or_dir = Dir{};
  if constexpr (std::same_as<File, T>) {
    size_or_dir = item.size;
  }
  dir.push_back(
    std::make_shared<Node>(Node{ .name = std::string{ item.name },
                                 .parent_dir = m_cwd,
                                 .size_or_dir = std::move(size_or_dir) }));
}
void
FileTree::change_directory(std::string_view dir_name)
{
  assert(m_cwd->is_dir());
  if (dir_name == ROOT_DIR) {
    m_cwd = m_root;
  } else if (dir_name == UP_DIR) {
    if (auto parent = m_cwd->parent_dir.lock()) {
      m_cwd = parent;
    }
  } else {
    const auto& dir = m_cwd->get_dir();
    const auto it = std::ranges::find_if(dir, [&dir_name](const auto& node) {
      return node->is_dir() && node->name == dir_name;
    });
    if (it == dir.end()) {
      throw std::runtime_error("Directory not found");
    }
    m_cwd = *it;
  }
}
std::vector<std::size_t>
FileTree::get_dir_sizes() const
{
  std::vector<std::size_t> sizes{};
  get_dir_sizes(*m_root, sizes);
  return sizes;
}

std::size_t
FileTree::get_size(const Node& node)
{
  if (node.is_dir()) {
    std::size_t total = 0;
    for (const auto& child : node.get_dir()) {
      total += get_size(*child);
    }
    return total;
  }
  return std::get<std::size_t>(node.size_or_dir);
}

void
FileTree::get_dir_sizes(const Node& node, std::vector<std::size_t>& sizes)
{
  if (node.is_dir()) {
    sizes.push_back(get_size(node));
    for (const auto& child : node.get_dir()) {
      get_dir_sizes(*child, sizes);
    }
  }
}

std::pair<std::size_t, std::size_t>
solve(const Input& input)
{
  FileTree ft{};
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

  auto dir_sizes = ft.get_dir_sizes();
  std::ranges::sort(dir_sizes, std::less<std::size_t>{});

  static constexpr std::size_t max_dir_size = 100000;
  auto dir_sizes_below_max_size =
    dir_sizes | std::ranges::views::filter(
                  [](const auto& size) { return size <= max_dir_size; });
  const std::size_t result_part1 =
    std::accumulate(dir_sizes_below_max_size.begin(),
                    dir_sizes_below_max_size.end(),
                    std::size_t{ 0 });

  static constexpr std::size_t total_filesystem_size = 70000000;
  static constexpr std::size_t needed_size = 30000000;
  const auto current_unused =
    total_filesystem_size - dir_sizes[dir_sizes.size() - 1];
  const auto size_to_delete = needed_size - current_unused;

  auto candidate_dir_sizes_to_delete =
    dir_sizes | std::ranges::views::filter(
                  [&](const auto& size) { return size >= size_to_delete; });
  const std::size_t result_part2 = *candidate_dir_sizes_to_delete.begin();

  return std::make_pair(result_part1, result_part2);
}
};
