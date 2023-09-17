#ifndef AOC22_DAY10_HPP
#define AOC22_DAY10_HPP

#include <cstdint>
#include <variant>
#include <vector>
#include <utility>

namespace aoc22::day10
{
    struct Noop{};
    
    struct AddX {
        std::int64_t x;
    };

    using Instruction = std::variant<Noop, AddX>;    

    using Input  = std::vector<Instruction>;
    using Result = std::pair<std::uint64_t, std::uint64_t>;

    Result solve(const Input& input); 
};

#endif