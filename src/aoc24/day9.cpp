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
void
printDiskMap(const DiskMap& diskMap)
{
  for (const auto& diskElement : diskMap) {
    if (diskElement.index() == 0) {
      const auto file = std::get<File>(diskElement);
      for (std::size_t i = 0; i < file.size; ++i) {
        std::print("{}", file.id);
      }
    } else {
      const auto size = std::get<Empty>(diskElement).size;
      for (std::size_t i = 0; i < size; ++i) {
        std::print(".");
      }
    }
  }
  std::println("");
}

auto
defragmentPart1(Input input)
{

  auto diskMap = getDiskMap(input);
  std::size_t nextEmptyIndex = 0;
  std::size_t nextFileIndex = diskMap.size() - 1;

  while (nextEmptyIndex != nextFileIndex) {
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
    auto& nextEmpty = std::get<Empty>(diskMap[nextEmptyIndex]);
    auto& nextFile = std::get<File>(diskMap[nextFileIndex]);
    const auto blocksToTransfer = std::min(nextEmpty.size, nextFile.size);
    nextEmpty.size -= blocksToTransfer;
    nextFile.size -= blocksToTransfer;
    auto it = diskMap.begin();
    std::advance(it, nextEmptyIndex);
    diskMap.insert(it, File{ nextFile.id, blocksToTransfer });
    ++nextEmptyIndex;
    ++nextFileIndex;
  }
  return filterDiskMap(diskMap);
}
auto
defragmentPart2(Input input)
{
  auto diskMap = getDiskMap(input);
  std::size_t nextEmptyIndex = 0;
  std::size_t nextFileIndex = diskMap.size() - 1;
  std::set<std::size_t> alreadyMoved{};

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
    
    if (empty.size >= file.size) {
      std::size_t size = file.size;
      std::size_t id = file.id;
      empty.size -= file.size;
      diskMap[nextFileIndex] = Empty{ size };
      alreadyMoved.insert(id);
      auto it = diskMap.begin();
      std::advance(it, nextEmptyIndex);
      diskMap.insert(it, File{ id, size });
      nextEmptyIndex = 0;
      continue;
    }
    ++nextEmptyIndex;
  }
  return filterDiskMap(diskMap);
}
}

Result
solve(Input input)
{
  const auto part1 = calculateCheckSum(defragmentPart1(input));
  const auto part2 = calculateCheckSum(defragmentPart2(input));
  return { part1, part2 };
}

}