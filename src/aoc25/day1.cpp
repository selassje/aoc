
#include "aoc25/day1.hpp"
#include <atomic>
#include <cstdint>
#include <iterator>
#include <limits>
#include <iostream>

namespace {

std::uint64_t rotate(std::uint64_t position, const aoc25::day1::Rotation &rotation) {
  constexpr auto maxCount = std::numeric_limits<std::uint64_t>::max();
  
  if ( rotation.direction == aoc25::day1::Direction::Right){
    //  std::cout <<"Ooops\n";
    if (rotation.count > maxCount - position) {
      std::cout <<"Ooops\n";
    }
    
    position = (position + rotation.count) % 100; //NOLINT
  } else {
    if (position >= rotation.count) {
      position -= rotation.count;
    }
    else {
      //position = 100 - ((rotation.count - position) % 100); //NOLINT
      auto count  = rotation.count;
      count -= position; 
      position = 0;
      position = (100 - ((count - position) % 100)) % 100; //NOLINT
    }

  }
  return position;
}

}


namespace aoc25::day1 {
Result
solve(const Input &input)
{
  std::uint64_t position = 50; //NOLINT
  std::uint64_t part1 = 0;

  for ( const auto& rotation : input) {
      position = rotate(position, rotation);
      if ( position == 0)
      {
        ++part1;
      }
  }
  return {part1, part1};
}
}