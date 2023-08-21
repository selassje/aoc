#include  <numeric>
#include  <algorithm>

namespace aoc22{
namespace day1 {   
    template<typename Iterator>
    auto solve(const Iterator begin, const Iterator end) noexcept
    {
        unsigned int maxCalories = 0;
        for(auto it = begin ; it !=  end; ++it)
        {
            unsigned int currentElfCalories  = std::accumulate(it->begin(), it->end(),0);
            maxCalories = std::max(maxCalories, currentElfCalories);
        }
        return maxCalories;
    }
}
}