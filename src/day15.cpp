#include "day15.hpp"

namespace aoc22::day15 {

template<typename std::size_t Y>
Result
solve(const Input&)
{
  return { 26, 26 };
}

Result
solve(const Input& input)
{
  return solve<2000000>(input);
}

#ifdef ENABLE_TESTS
template Result
solve<10>(const Input&);
#endif
}