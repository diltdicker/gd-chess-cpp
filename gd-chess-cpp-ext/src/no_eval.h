#ifndef NO_EVAL_H
#define NO_EVAL_H

#include "chess_logic.h"
#include "eval_strategy.h"

class NoEvalStrategy : public EvaluationStrategy {

public:
    NoEvalStrategy() = default;

    // Evaluate the board position and return a score
    int evaluate(const ChessLogic::chessPiece* board) override {
        return 0; // No evaluation, return 0
    }
};

#endif // NO_EVAL_H