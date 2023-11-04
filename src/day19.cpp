#include "day19.hpp"

#include <queue>

namespace aoc22::day19 {

struct State
{
  int minutes{ 20 };
  int ore{};
  int clay{};
  int obsidian{};
  int geodes{};
  int robotCount[4] = { 0, 0, 0, 0 };
  int newRobotCount[4] = { 1, 0, 0, 0 };
};

auto
findMaxGeodes(const Blueprint& blueprint)
{
  int maxGeodes = 0;

  std::queue<State> statesQueue{};
  statesQueue.push({});

  while (!statesQueue.empty()) {
    auto state = statesQueue.front();
    statesQueue.pop();
    state.ore += state.robotCount[0];
    state.clay += state.robotCount[1];
    state.obsidian += state.robotCount[2];
    state.geodes += state.robotCount[3];

    maxGeodes = std::max(maxGeodes, state.geodes);

    if (state.minutes >= 0) {
      const auto maxNewOreRobots = state.ore / blueprint.oreRobotCost.ore;
      const auto maxNewClayRobots = state.ore / blueprint.clayRobotCost.ore;
      const auto maxNewObsidianRobots =
        std::min(state.ore / blueprint.obsidianRobotCost.ore,
                 state.clay / blueprint.obsidianRobotCost.clay);
      const auto maxNewGeodesRobots =
        std::min(state.ore / blueprint.geodeRobotCost.ore,
                 state.obsidian / blueprint.geodeRobotCost.obsidian);

      for (int oreRobots = 0; oreRobots <= maxNewOreRobots; ++oreRobots)
        for (int clayRobots = 0; clayRobots <= maxNewClayRobots; ++clayRobots)
          for (int obsidianRobots = 0; obsidianRobots <= maxNewObsidianRobots;
               ++obsidianRobots)
            for (int geodesRobots = 0; geodesRobots <= maxNewGeodesRobots;
                 ++geodesRobots) {
              const auto oreCost =
                oreRobots * blueprint.oreRobotCost.ore +
                clayRobots * blueprint.clayRobotCost.ore +
                obsidianRobots * blueprint.obsidianRobotCost.ore +
                geodesRobots * blueprint.geodeRobotCost.ore;

              const auto clayCost =
                obsidianRobots * blueprint.obsidianRobotCost.clay;
              const auto obsidianCost =
                geodesRobots * blueprint.geodeRobotCost.obsidian;

              if (state.ore >= oreCost && state.clay >= clayCost &&
                  state.obsidian >= obsidianCost) {

                auto nextState = state;
                nextState.minutes--;
                nextState.ore -= oreCost;
                nextState.clay -= clayCost;
                nextState.obsidian -= obsidianCost;
                for (int i = 0; i < 4; ++i) {
                  nextState.robotCount[i] += state.newRobotCount[i];
                }
                nextState.newRobotCount[0] = oreRobots;
                nextState.newRobotCount[1] = clayRobots;
                nextState.newRobotCount[2] = obsidianRobots;
                nextState.newRobotCount[3] = geodesRobots;

                statesQueue.push(nextState);
              }
            }
    }
  }
  return maxGeodes;
};

Result
solve(const Input& input)
{
  std::size_t part1 = 0;
  for (const auto& bluepint : input) {
    part1 += static_cast<std::size_t>(bluepint.id * findMaxGeodes(bluepint));
  }
  return { part1, part1 };
}
}