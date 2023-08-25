#include "day2.hpp"
#include <numeric>
#include <unordered_map>
#include <vector>
namespace aoc22::day2
{
    enum class Outcome : unsigned char
    {
        Loss,
        Draw = 3,
        Win = 6,
    };

    namespace 
    {
        using enum Move;
        
        constexpr unsigned int evaluateMove(Move m) {
            return static_cast<unsigned int>(m);
        }
        
        const std::unordered_map<Move,Move> beats{ {Rock, Sciscors}, {Sciscors,Paper},{Paper,Rock} }; 

        constexpr unsigned int evaluateOutcome(Move oponent, Move you) {
            using enum Outcome;
            
            Outcome outcome {Draw};

            if (oponent != you) 
            {  
                outcome = beats.at(you) == oponent ? Win : Loss;
            }
            return static_cast<unsigned int>(outcome);
        }
    }

    unsigned int solve(const Guide &guide) noexcept
    {
        auto evaluateRound = [](Move oponent, Move you) {
            return evaluateMove(you) + evaluateOutcome(oponent, you);
        };
        unsigned int score = 0;
        for ( const auto& [oponent, you] : guide )
        {
            score += evaluateRound(oponent,you);
        }
        return score;
    }
}

