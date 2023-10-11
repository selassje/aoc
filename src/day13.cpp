#include "day13.hpp"

#include <algorithm>
#include <memory>
#include <ranges>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace aoc22::day13 {

struct List;

using Packet = std::variant<std::shared_ptr<List>, std::size_t>;

struct List
{
  std::vector<Packet> packets;
};

Packet
parse(std::string_view packet)
{
  if (packet[0] == '[') {
    auto listPtr = std::make_shared<List>();
    std::size_t nextSubpacketStart = 0;
    std::size_t level = 0;

    for (std::size_t i = 1; i < packet.size(); ++i) {
      if ((packet[i] == ',' || i == packet.size() - 1) && level == 0) {
        if (const auto subPacket =
              packet.substr(nextSubpacketStart + 1, i - nextSubpacketStart - 1);
            !subPacket.empty()) {
          listPtr->packets.push_back(parse(subPacket));
        }
        nextSubpacketStart = i;
      } else if (packet[i] == '[') {
        ++level;
      } else if (packet[i] == ']') {
        --level;
      }
    }
    return listPtr;
  } else {
    const auto integerEnd = std::ranges::find_if(
      packet, [](char token) { return token == ',' || token == ']'; });
    std::string integer{ packet.begin(), integerEnd };
    return static_cast<std::size_t>(
      std::strtoull(integer.c_str(), nullptr, 10));
  }
}

enum class CmprResult
{
  Lesser,
  Greater,
  Equal,
};

using enum CmprResult;

CmprResult
compare(const Packet& left, const Packet& right)
{
  CmprResult result = Equal;
  if (left.index() == 1 && right.index() == 1) {
    const auto leftInteger = std::get<1>(left);
    const auto rightInteger = std::get<1>(right);
    if (leftInteger < rightInteger) {
      result = Lesser;
    } else if (leftInteger > rightInteger) {
      result = Greater;
    }
  } else if (left.index() == 0 && right.index() == 0) {
    const auto& leftList = std::get<0>(left);
    const auto& rightList = std::get<0>(right);
    const auto minSize =
      std::min(leftList->packets.size(), rightList->packets.size());
    for (std::size_t i = 0; i < minSize; ++i) {
      const auto cmprResult =
        compare(leftList->packets[i], rightList->packets[i]);
      if (cmprResult != Equal) {
        result = cmprResult;
        break;
      }
    }
    if (result == Equal) {
      if (leftList->packets.size() < rightList->packets.size()) {
        result = Lesser;
      } else if (leftList->packets.size() > rightList->packets.size()) {
        result = Greater;
      }
    }
  } else if (left.index() == 1) {
    auto listPtr = std::make_shared<List>();
    listPtr->packets.emplace_back(std::get<1>(left));
    result = compare(std::move(listPtr), right);
  } else {
    auto listPtr = std::make_shared<List>();
    listPtr->packets.emplace_back(std::get<1>(right));
    result = compare(left, std::move(listPtr));
  }
  return result;
}

Result
solve(const Input& input)
{
  const auto divider1 = parse("[[2]]");
  const auto divider2 = parse("[[6]]");

  std::size_t resultPart1 = 0;
  std::vector elements{ divider1, divider2 };
  for (std::size_t i = 0; i < input.size(); ++i) {
    const auto& [first, second] = input[i];
    auto firstParsed = parse(first);
    auto secondParsed = parse(second);
    if (compare(firstParsed, secondParsed) == Lesser) {
      resultPart1 += i + 1;
    }
    elements.emplace_back(firstParsed);
    elements.emplace_back(secondParsed);
  }

  std::ranges::sort(elements, [](const auto& left, const auto& right) {
    return compare(left, right) == Lesser;
  });

  auto findIndex = [&elements](const auto& packet) {
    const auto index =
      std::ranges::find_if(elements, [&packet](const auto& packet_) {
        return compare(packet, packet_) == Equal;
      });
    return static_cast<std::size_t>(index - elements.begin()) + 1;
  };

  const auto divider1Index = findIndex(divider1);
  const auto divider2Index = findIndex(divider2);

  return { resultPart1, divider1Index * divider2Index };
}

}