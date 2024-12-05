
#include "day5.hpp"

#include "inputs.hpp"
#include "parsing.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <ranges>
#include <string_view>
#include <utility>
#include <vector>

namespace   {
  aoc24::day5::Input readInput(std::string_view path) {
      aoc24::day5::Input input{};
      aoc24::day5::Orders orders{};
      aoc24::day5::Updates updates{};
      inputs::FileReader fileReader{path};
      bool readOrders = true;
      while (auto line = fileReader.readLine()) {
          if ( *line == "" ) {
              readOrders = false;
              continue;
          }
          if ( readOrders) {
              std::replace(std::begin(*line), std::end(*line),'|',' ');
              const auto order = inputs::parseLine<std::size_t, std::size_t>(*line);
              orders.emplace_back(order);
          } else {
              std::replace(std::begin(*line), std::end(*line),',',' ');
              const auto update = inputs::parseStringDynamic<std::size_t>(*line);
              updates.emplace_back(update);
          }      
      }
      return {orders,updates};
  }
}

TEST_CASE("Aoc24 Day4 Example", "[AoC24_Day4]")
{
  const auto input = readInput(inputs::day5::EXAMPLE);
  const auto [part1, _] = aoc24::day5::solve(input);
  REQUIRE(part1 == 143);
  //REQUIRE(part2 == 9);
}