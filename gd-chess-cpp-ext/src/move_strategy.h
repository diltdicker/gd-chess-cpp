#ifndef MOVE_STRATEGY_H
#define MOVE_STRATEGY_H

#include "chess_logic.h"
#include "eval_strategy.h"

class MoveStrategy {
    public:

        virtual ~MoveStrategy() = default;

        virtual ChessLogic::Move getBestMove(ChessLogic &logic, EvaluationStrategy* evalStrategy,
             bool isWhite, long ms_time) = 0; 
};

#endif