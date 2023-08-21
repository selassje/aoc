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

static bool test(unsigned int expected_part1, unsigned int expected_part2, std::string_view input_file )
{
    auto input = readInput(input_file);
    auto [part_1, part_2] = aoc22::day1::solve(input);
    return expected_part1 == part_1 && expected_part2 == part_2;
}


int main() 
{
    return test(24000, 45000,  inputs::day1::EXAMPLE) &&
           test(70613, 205805, inputs::day1::INPUT) ?  0 : 1;

}
