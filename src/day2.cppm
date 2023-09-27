module;

#include <utility>
#include <vector>

export module Day2;

//import <vector;

export namespace aoc22::day2 {

enum class Move : unsigned char
{
  Rock = 1,
  Paper,
  Sciscors
};

using Guide = std::vector<std::pair<Move, Move>>;

std::pair<unsigned int, unsigned int>
solve(const Guide& guide);

}
