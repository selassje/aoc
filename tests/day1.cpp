#include <vector>
#include <string_view>
#include <fstream>
#include <sstream>
#include <iostream>


#include "inputs.hpp"

using std::vector;
using NestedVector = vector<vector<unsigned int>>;

static NestedVector readInput(const std::string_view path) {

    std::ifstream ifs{"F:\\Repos\\aoc22\\tests\\input\\day1\\example.txt"};
    //std::ifstream ifs{"example.txt"};
    auto r = ifs.rdstate();
    
    
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
    return input;
}


int main(int argc, const char *argv[]) {

    std::cout << "Hello World" << std::endl;

    auto input = readInput(inputs::day1::EXAMPLE);
    for (const auto &elf : input)
    {
        for(const auto &calories : elf)
        {
            std::cout << calories << std::endl;
        }
        std::cout << std::endl;
    }
    return 1;
}
