#ifndef RANDOM_MOVE_H
#define RANDOM_MOVE_H

#include <vector>
#include "chess_logic.h"
#include "move_strategy.h"
#include "eval_strategy.h"

class RandomMoveStrategy : public MoveStrategy {

public:
    RandomMoveStrategy() = default;

    ChessLogic::Move getBestMove(ChessLogic &logic, EvaluationStrategy* evalStrategy,
                                 bool isWhite, long ms_time) override;
};

#endif