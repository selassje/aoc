#include "inputs.hpp"
#include "day1.hpp"

#include <vector>
#include <string_view>
#include <fstream>
#include <sstream>
#include <iostream>


using std::vector;
using NestedVector = vector<vector<unsigned int>>;

static NestedVector readInput(const std::string_view path) {

    std::ifstream ifs{path.data()};
       
    NestedVector input;
    vector<unsigned int> caloriesPerElf{};

    std::string line{};
    while(std::getline(ifs,line))
    {
        if(line.empty())
        {
            input.push_back(caloriesPerElf);
            caloriesPerElf.clear();    
        }
        else
        {
            std::istringstream iss{line};
            unsigned int calories;
            iss >> calories;
            caloriesPerElf.push_back(calories);
        }
    }
    input.push_back(caloriesPerElf);
    return input;
}

static bool test(unsigned int expected, std::string_view input_file )
{
    auto input = readInput(input_file);
    return expected == aoc22::day1::solve(input.begin(), input.end());
}


int main() 
{
    return test(24000, inputs::day1::EXAMPLE) &&
           test(70613, inputs::day1::INPUT) ?  0 : 1;

}
