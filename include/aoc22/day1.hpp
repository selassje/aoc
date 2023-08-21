#include  <numeric>
#include  <algorithm>

namespace day1 {
    
    template<typename R, typename Iterator>
    R solve(const Iterator &begin, const Iterator &end) noexcept
    {
        R maxCalories = 0;
        for(auto it = begin ; it !=  end; ++it)
        {
            R currentElfCalories  = std::accumulate(it->begin(), it->end());
            maxCalories = std::max(maxCalories, currentElfCalories);
        }
        return maxCalories;
    }
}