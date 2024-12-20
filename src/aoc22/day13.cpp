#include "aoc22/day13.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace aoc22::day13 {

struct List;

using Packet = std::variant<std::unique_ptr<List>, std::size_t>;

struct List
{
  std::vector<Packet> packets;
};

namespace {

Packet
parse(std::string_view packet)
{
  if (packet[0] == '[') {
    auto listPtr = std::make_unique<List>();
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
  }
  const auto integerEnd = std::ranges::find_if( // NOLINT
    packet,
    [](char token) { return token == ',' || token == ']'; });
  const std::string integer{ packet.begin(), integerEnd };
  static constexpr auto base = 10;
  return static_cast<std::size_t>(
    std::strtoull(integer.c_str(), nullptr, base));
}

enum class CmprResult : std::uint8_t
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
  if (left.index() == 1 && right.index() == 1) { // NOLINT
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
    auto listPtr = std::make_unique<List>();
    listPtr->packets.emplace_back(std::get<1>(left));
    result = compare(std::move(listPtr), right);
  } else {
    auto listPtr = std::make_unique<List>();
    listPtr->packets.emplace_back(std::get<1>(right));
    result = compare(left, std::move(listPtr));
  }
  return result;
}
}
Result
solve(const Input& input)
{
  static constexpr std::string_view divider1 = "[[2]]";
  static constexpr std::string_view divider2 = "[[6]]";

  std::size_t resultPart1 = 0;
  std::vector<Packet> packets{};
  packets.emplace_back(parse(divider1));
  packets.emplace_back(parse(divider2));
  for (std::size_t i = 0; i < input.size(); ++i) {
    const auto& [first, second] = input[i];
    auto firstParsed = parse(first);
    auto secondParsed = parse(second);
    if (compare(firstParsed, secondParsed) == Lesser) {
      resultPart1 += i + 1;
    }
    packets.emplace_back(std::move(firstParsed));
    packets.emplace_back(std::move(secondParsed));
  }
  std::ranges::sort(packets, [](const auto& left, const auto& right) { // NOLINT
    return compare(left, right) == Lesser;
  });

  auto findIndex = [&packets](const auto& packet) {
    const auto index =
      std::ranges::find_if(packets, [&packet](const auto& packet_) {
        return compare(parse(packet), packet_) == Equal;
      });
    return static_cast<std::size_t>(index - packets.begin()) + 1;
  };

  const auto divider1Index = findIndex(divider1);
  const auto divider2Index = findIndex(divider2);

  return { resultPart1, divider1Index * divider2Index };
}
}
