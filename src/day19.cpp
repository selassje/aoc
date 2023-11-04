#include "day19.hpp"

namespace aoc22::day19 {

  auto findMaxGeodes(const Blueprint& blueprint)  {
      auto maxGeodes = blueprint.clayRobotCost.ore;
      return maxGeodes;
  };

  Result solve(const Input& input) {
    std::size_t part1 = 0;
    for ( const auto& bluepint : input) {
      part1 += static_cast<std::size_t>(bluepint.id * findMaxGeodes(bluepint));
    }
    return {part1,part1};
  }
}