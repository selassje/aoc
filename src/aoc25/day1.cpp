
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
std::uint64_t rotate2(std::uint64_t position, const aoc25::day1::Rotation &rotation) {
  constexpr auto maxCount = std::numeric_limits<std::uint64_t>::max();
  
  if ( rotation.direction == aoc25::day1::Direction::Right){
    //  std::cout <<"Ooops\n";
    if (rotation.count > maxCount - position) {
      std::cout <<"Ooops\n";
    }
    return  (position + rotation.count) / 100; //NOLINT
  } else {
    if (position >= rotation.count) {
      position -= rotation.count;
      if(position == 0) {
        return 1;
      }
    }
    else {
      auto count  = rotation.count;
      //position = 100 - ((rotation.count - position) % 100); //NOLINT
      if ( position == 0) {
        return count/100;
      }

      count -= position;
      std::uint64_t result = 1; 
      position = 0;
      position = (100 - ((count - position) % 100)) % 100; //NOLINT
      return (count/100) + 1;    //NOLINT
    }

  }
  return 0;
}

}


namespace aoc25::day1 {
Result
solve(const Input &input)
{
  std::uint64_t position = 50; //NOLINT
  std::uint64_t part1 = 0;
  std::uint64_t part2 = 0;

  for ( const auto& rotation : input) {
      const auto positionCpy = position;
      part2 += rotate2(positionCpy, rotation);
      position = rotate(position, rotation);
      if ( position == 0)
      {
        ++part1;
      }
  }
  return {part1, part2};
}
}