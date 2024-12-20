#include "aoc22/day7.hpp"

#include <algorithm>
#include <cassert>
#include <concepts> //NOLINT
#include <cstddef>
#include <memory>
#include <numeric>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace aoc22::day7 {
template<typename... Ts>
struct Overload : Ts... // NOLINT
{
  using Ts::operator()...;
};
class FileTree
{
public:
  template<typename T>
  void addItem(const T& item) const;

  void changeDirectory(std::string_view dir_name);

  [[nodiscard]] std::vector<std::size_t> getDirSizes() const;

private:
  struct Node;
  using Dir = std::vector<std::shared_ptr<Node>>;
  using SizeOrDir = std::variant<Dir, std::size_t>;
  struct alignas(128) Node
  {
    std::string name;
    std::weak_ptr<Node> parentDir;
    SizeOrDir sizeOrDir;

    [[nodiscard]] bool isDir() const noexcept { return sizeOrDir.index() == 0; }
    [[nodiscard]] const Dir& getDir() const { return std::get<Dir>(sizeOrDir); }
    [[nodiscard]] Dir& getDir() { return std::get<Dir>(sizeOrDir); }

    explicit Node(std::string name_)
      : name{ std::move(name_) } {};

    explicit Node(std::string name_,
                  std::weak_ptr<Node> parentDir_,
                  SizeOrDir sizeOrDir_)
      : name{ std::move(name_) }
      , parentDir{ std::move(parentDir_) }
      , sizeOrDir{ std::move(sizeOrDir_) } {};
  };

  std::shared_ptr<Node> m_root =
    std::make_shared<Node>(std::string{ ROOT_DIR });
  std::shared_ptr<Node> m_cwd{ m_root };

  static constexpr std::string_view ROOT_DIR = "/";
  static constexpr std::string_view UP_DIR = "..";

  static std::size_t getSize(const Node& node);
  static void getDirSizes(const Node& node, std::vector<std::size_t>& sizes);
};

template<typename T>
void
FileTree::addItem(const T& item) const
{
  assert(m_cwd->isDir());
  auto& dir = m_cwd->getDir();
  if (const auto it = std::ranges::find_if(
        dir, [&item](const auto& node) { return node->name == item.name; });
      it != dir.end()) {
    throw std::runtime_error("Element already exists!");
  }

  SizeOrDir sizeOrDir = Dir{}; // NOLINT
  if constexpr (std::same_as<File, T>) {
    sizeOrDir = item.size;
  }
  dir.emplace_back(std::make_shared<Node>(item.name, m_cwd, sizeOrDir));
}
void
FileTree::changeDirectory(std::string_view dir_name)
{
  assert(m_cwd->isDir());
  if (dir_name == ROOT_DIR) {
    m_cwd = m_root;
  } else if (dir_name == UP_DIR) {
    if (auto parent = m_cwd->parentDir.lock()) {
      m_cwd = parent;
    }
  } else {
    const auto& dir = m_cwd->getDir();
    const auto it = std::ranges::find_if(dir, [&dir_name](const auto& node) {
      return node->isDir() && node->name == dir_name;
    });
    if (it == dir.end()) {
      throw std::runtime_error("Directory not found");
    }
    m_cwd = *it;
  }
}
std::vector<std::size_t>
FileTree::getDirSizes() const
{
  std::vector<std::size_t> sizes{};
  getDirSizes(*m_root, sizes);
  return sizes;
}

std::size_t
FileTree::getSize(const Node& node)
{
  if (node.isDir()) {
    std::size_t total = 0;
    for (const auto& child : node.getDir()) {
      total += getSize(*child);
    }
    return total;
  }
  return std::get<std::size_t>(node.sizeOrDir);
}

void
FileTree::getDirSizes(const Node& node, std::vector<std::size_t>& sizes)
{
  if (node.isDir()) {
    sizes.push_back(getSize(node));
    for (const auto& child : node.getDir()) {
      getDirSizes(*child, sizes);
    }
  }
}

std::pair<std::size_t, std::size_t>
solve(const Input& input)
{
  FileTree ft{};

  // clang-format off
  auto recordHandler =
    Overload{ [&ft](const ChangeDirectory &cd) { ft.changeDirectory(cd.directory); },
              [&ft](const Directory &dir ) { ft.addItem(dir); },
              [&ft](const File &file ) { ft.addItem(file); }, 
              [](const List& ) { /*NOACTION*/ }
    };
  // clang-format on
  for (const auto& record : input) {
    std::visit(recordHandler, record);
  }

  auto dirSizes = ft.getDirSizes();
  std::ranges::sort(dirSizes);
#ifdef _MSC_VER
#pragma warning(suppress : 5264)
#endif
  static constexpr std::size_t maxDirSize = 100000;
  auto dirSizesBelowMaxSize =
    dirSizes | std::ranges::views::filter(
                 [](const auto& size) { return size <= maxDirSize; });
  const std::size_t resultPart1 = std::accumulate(
    dirSizesBelowMaxSize.begin(), dirSizesBelowMaxSize.end(), std::size_t{ 0 });

  static constexpr std::size_t totalFilesystemSize = 70000000;
  static constexpr std::size_t neededSize = 30000000;
  const auto currentUnused =
    totalFilesystemSize - dirSizes[dirSizes.size() - 1];
  const auto sizeToDelete = neededSize - currentUnused;

  auto candidateDirSizesToDelete =
    dirSizes | std::ranges::views::filter([sizeToDelete](const auto& size) {
      return size >= sizeToDelete;
    });
  const std::size_t resultPart2 = *candidateDirSizesToDelete.begin();

  return std::make_pair(resultPart1, resultPart2);
}
};
