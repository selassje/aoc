#include "aoc24/day5.hpp"
#include <cstddef>
#include <iterator>

namespace aoc24::day5 {

namespace {

bool
arePagesInOrder(std::size_t page1, std::size_t page2, const Orders& orders)
{
  for (const auto& order : orders) {
    if (order.first == page2 && order.second == page1) {
      return false;
    }
  }

  return true;
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

std::size_t
getMiddlePage(const Update& update)
{
  return update[update.size() / 2];
}

}

Result
solve(const Input& input)
{
  std::size_t part1 = 0;
  for (const auto& update : input.updates) {
    if (isUpdateInOrder(update, input.orders)) {
      part1 += getMiddlePage(update);
    }
  }

  return { part1, part1 };
}

}
