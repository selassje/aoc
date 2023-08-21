#include <numeric>
#include <algorithm>
#include <vector>
#include <utility>

namespace aoc22{
namespace day1 {   
    template<typename NestedRange>
    auto get_top_three_calories_per_elf (const NestedRange& input) noexcept
    {
        std::vector<unsigned int> caloriesPefElf{};
        for(const auto& inner : input)
        {
            unsigned int currentElfCalories  = std::reduce(inner.begin(), inner.end(),0);
            caloriesPefElf.push_back(currentElfCalories);
        }
        std::ranges::partial_sort(caloriesPefElf, caloriesPefElf.begin() + 3, [](auto a, auto b) {return a > b;} );
        return std::vector<unsigned int>{caloriesPefElf.begin(), caloriesPefElf.begin() + 3};
    }
   
    template<typename NestedRange>
    auto solve(const NestedRange& input) noexcept
    {
        const auto top_three = get_top_three_calories_per_elf(input);
        const auto sum = std::reduce(top_three.begin(), top_three.end(),0u);
        const auto max = top_three[0];
        return std::make_pair(max, sum);
    }


}
}