#include <numeric>
#include <algorithm>
#include <vector>
#include <utility>

namespace aoc22{
namespace day1 {   
    template<typename Iterator>
    auto get_top_three_calories_per_elf (const Iterator begin, const Iterator end) noexcept
    {
        std::vector<unsigned int> caloriesPefElf{};
        for(auto it = begin ; it !=  end; ++it)
        {
            unsigned int currentElfCalories  = std::accumulate(it->begin(), it->end(),0);
            caloriesPefElf.push_back(currentElfCalories);
        }
        std::partial_sort(caloriesPefElf.begin(), caloriesPefElf.begin() + 3, caloriesPefElf.end(), [](auto a, auto b) {return a > b;} );

        return std::vector<unsigned int>{caloriesPefElf.begin(), caloriesPefElf.begin() + 3};
    }
   
    template<typename Iterator>
    auto solve(const Iterator begin, const Iterator end) noexcept
    {
        const auto top_three = get_top_three_calories_per_elf(begin, end);
        const auto sum = std::accumulate(top_three.begin(), top_three.end(),0u);
        const auto max = top_three[0];
        return std::make_pair(max, sum);
    }


}
}