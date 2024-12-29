#include "aoc24/day9.hpp"
#include <cstddef>
#include <iterator>
#include <optional>
#include <print>
#include <ranges>
#include <set>
#include <utility>
#include <variant>
#include <vector>

namespace aoc24::day9 {

namespace {

struct File
{
  std::size_t id;
  std::size_t size;
};

struct Empty
{
  std::size_t size;
};

using DiskElement = std::variant<File, Empty>;

using DiskMap = std::vector<DiskElement>;

auto
getDiskMap(Input input)
{
  DiskMap map{};
  std::size_t id = 0;
  for (std::size_t i = 0; i < input.size(); ++i) {
    const auto size = static_cast<std::size_t>(input[i]);
    if (size != 0) {
      if (i % 2 == 0) {
        map.emplace_back(File{ id, size });
        ++id;
      } else {
        map.emplace_back(Empty{ size });
      }
    }
  }
  return map;
}

auto
calculateCheckSum(const DiskMap& diskMap)
{
  std::size_t checkSum = 0;
  std::size_t index = 0;
  for (const auto& diskElement : diskMap) {
    if (diskElement.index() == 0) {
      const auto file = std::get<File>(diskElement);
      for (std::size_t i = index; i < index + file.size; ++i) {
        checkSum += i * file.id;
      }
      index += file.size;
    } else {
      index += std::get<Empty>(diskElement).size;
    }
  }
  return checkSum;
}
auto
filterDiskMap(const DiskMap& diskMap)
{
  auto emptyElem = [](const DiskElement& elem) {
    return (elem.index() == 0 && std::get<File>(elem).size != 0) ||
           (elem.index() == 1 && std::get<Empty>(elem).size != 0);
  };

  auto filtered = diskMap | std::ranges::views::filter(emptyElem) |
                  std::ranges::to<std::vector>();

  return filtered;
}

template<bool moveOnlyWholeFiles>
auto
defragment(Input input)
{
  auto diskMap = getDiskMap(input);

  std::size_t nextEmptyIndex = 0;
  std::size_t nextFileIndex = diskMap.size() - 1;

  while (nextFileIndex > 0) {
    if (nextEmptyIndex >= nextFileIndex) {
      nextEmptyIndex = 0;
      --nextFileIndex;
      continue;
    }
    if (diskMap[nextEmptyIndex].index() == 0 ||
        std::get<Empty>(diskMap[nextEmptyIndex]).size == 0) {
      ++nextEmptyIndex;
      continue;
    }
    if (diskMap[nextFileIndex].index() == 1 ||
        std::get<File>(diskMap[nextFileIndex]).size == 0) {
      --nextFileIndex;
      continue;
    }
    auto& empty = std::get<Empty>(diskMap[nextEmptyIndex]);
    auto& file = std::get<File>(diskMap[nextFileIndex]);
    std::size_t size = 0;
    if constexpr (moveOnlyWholeFiles) {
      size = file.size;
    } else {
      size = std::min(empty.size, file.size);
    }

    if (empty.size >= size) {
      const std::size_t id = file.id;
      empty.size -= size;
      file.size -= size;
      if (file.size != 0) {
        nextFileIndex++;
      } else {
        diskMap[nextFileIndex] = Empty{ size };
      }
      auto it = diskMap.begin();
      std::advance(it, nextEmptyIndex);
      diskMap.insert(it, File{ id, size });
      nextEmptyIndex = 0;
      continue;
    }
    if constexpr (moveOnlyWholeFiles) {
      ++nextEmptyIndex;
    }
  }
  return filterDiskMap(diskMap);
}
}

Result
solve(Input input)
{
  const auto part1 = calculateCheckSum(defragment<false>(input));
  const auto part2 = calculateCheckSum(defragment<true>(input));
  return { part1, part2 };
}

}