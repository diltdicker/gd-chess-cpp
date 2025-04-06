#ifndef BEST_EVAL_MOVE_H
#define BEST_EVAL_MOVE_H

#include <vector>
#include <chrono>
#include "chess_logic.h"
#include "move_strategy.h"

#ifdef DEBUG
#define DEBUG_PRINT(x) std::cout << "Debug: " << x << "\n";
#else
#define DEBUG_PRINT(x)
#endif


class BestEvalMoveStrategy : public MoveStrategy {
public:
    BestEvalMoveStrategy() = default;

    ChessLogic::Move getBestMove(ChessLogic &logic, EvaluationStrategy* evalStrategy,
                                 bool isWhite, short searchDepth, 
                                 std::chrono::time_point<std::chrono::steady_clock> stopTime) override;


protected:
    int minimax(ChessLogic &logic, EvaluationStrategy* evalStrategy, bool isWhite, 
                short depth, std::chrono::time_point<std::chrono::steady_clock> stopTime);


    int minimax2(ChessLogic &logic, EvaluationStrategy* evalStrategy, bool isWhite, 
        short depth, std::chrono::time_point<std::chrono::steady_clock> stopTime);
};

#endif