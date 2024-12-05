#include "aoc24/day5.hpp"
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <utility>
#include <vector>

namespace aoc24::day5 {

namespace {

using Indexes = Update;

bool
arePagesInOrder(std::size_t page1, std::size_t page2, const Orders& orders)
{
  return std::ranges::all_of(orders, [&](const auto& order) {
    return !(order.first == page2 && order.second == page1);
  });
}

bool
isPageInOrder(std::size_t pageIndex, const Update& update, const Orders& orders)
{
  for (std::size_t i = 0; i < pageIndex; ++i) {
    if (!arePagesInOrder(update[i], update[pageIndex], orders)) {
      return false;
    }
  }
  for (std::size_t i = pageIndex + 1; i < update.size(); ++i) {
    if (!arePagesInOrder(update[pageIndex], update[i], orders)) {
      return false;
    }
  }

  return true;
}

bool
isUpdateInOrder(const Update& update, const Orders& orders)
{
  for (std::size_t i = 0; i < update.size(); ++i) {
    if (!isPageInOrder(i, update, orders)) {
      return false;
    }
  }
  return true;
}

Indexes
getOutOfOrderPageIndexes(const Update& update, const Orders& orders)
{
  Indexes result{};
  for (std::size_t i = 0; i < update.size(); ++i) {
    if (!isPageInOrder(i, update, orders)) {
      result.push_back(i);
    }
  }
  return result;
}

std::size_t
getMiddlePage(const Update& update)
{
  return update[update.size() / 2];
}

Update
correctUpdate(const Update& update, const Orders& orders)
{
  auto correctedUpdate = update;
  const auto outOfOrderIndexes = getOutOfOrderPageIndexes(update, orders);
  Update outOfOrderPages{};
  for (const auto i : outOfOrderIndexes) {
    outOfOrderPages.push_back(update[i]);
    std::erase(correctedUpdate, update[i]);
  }

  for (const auto& page : outOfOrderPages) {
    for (std::size_t i = 0; i <= correctedUpdate.size(); ++i) {
      auto correctedCpy = correctedUpdate;
      auto it = correctedCpy.begin();
      std::advance(it, i);
      correctedCpy.insert(it, page);
      if (isUpdateInOrder(correctedCpy, orders)) {
        std::swap(correctedCpy, correctedUpdate);
        break;
      }
    }
  }
  return correctedUpdate;
}

}

Result
solve(const Input& input)
{
  std::size_t part1 = 0;
  std::size_t part2 = 0;
  for (const auto& update : input.updates) {
    if (isUpdateInOrder(update, input.orders)) {
      part1 += getMiddlePage(update);
    } else {
      const auto correctedUpdate = correctUpdate(update, input.orders);
      part2 += getMiddlePage(correctedUpdate);
    }
  }

  return { part1, part2 };
}

}
