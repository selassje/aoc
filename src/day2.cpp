#include "day2.hpp"
#include <numeric>
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
        constexpr unsigned int evaluateMove(Move m) {
            return static_cast<unsigned int>(m);
        }

        constexpr unsigned int evaluateOutcome(Move oponent, Move you) {
            using enum Outcome;
            using enum Move;

            Outcome outcome {Draw};

            if (oponent != you) 
            {  
                if ( (you == Rock && oponent == Sciscors) || (you == Sciscors && oponent == Paper) || (you == Paper && oponent == Rock) )
                {
                    outcome = Win;
                }
                else
                {
                    outcome = Loss;
                }

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

