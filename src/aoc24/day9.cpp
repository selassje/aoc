#include "aoc24/day9.hpp"
#include <cstddef>
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
    if (i % 2 == 0) {
      map.emplace_back(File{ id, static_cast<std::size_t>(input[i]) });
      ++id;
    } else {
      map.emplace_back(Empty{ static_cast<std::size_t>(input[i]) });
    }
  }
  return map;
}

auto
calculateCheckSum(const DiskMap& diskMap)
{
  std::size_t checkSum = 0;
  for (std::size_t i = 0; i < diskMap.size(); ++i) {
    const auto& diskElement = diskMap[i];
    if (diskElement.index() == 0) {
      const auto file = std::get<File>(diskElement);
      checkSum += i * file.id;
    }
  }
  return checkSum;
}

}

Result
solve(Input input)
{
  auto diskMap = getDiskMap(input);

  std::size_t nextEmptyIndex = 0;
  std::size_t nextFileIndex = input.size() - 1;

  while (nextEmptyIndex != nextFileIndex) {
    if ( diskMap[nextEmptyIndex].index() == 0){
      ++nextEmptyIndex;
      continue;
    }
    if ( diskMap[nextFileIndex].index() == 1){
      --nextFileIndex;
      continue;
    }
    auto &nextEmpty = std::get<Empty>(diskMap[nextEmptyIndex]);
    auto &nextFile = std::get<File>(diskMap[nextFileIndex]);

    const auto blocksToTransfer = std::min(nextEmpty.size, nextFile.size);
    if(nextEmpty.size == blocksToTransfer) {
//      diskMap[nextEmptyIndex] = File

    } else {
      nextEmpty.size -= blocksToTransfer;
    }

    nextFile.size -= blocksToTransfer;
    if(nextFile.size == 0 ) {
      diskMap[nextFileIndex] = Empty{0};
    }


  }
  const auto part1 = calculateCheckSum(diskMap);
  return { part1, part1 };
}

}