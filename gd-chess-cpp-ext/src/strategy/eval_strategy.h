#ifndef EVALUATION_STRATEGY_H
#define EVALUATION_STRATEGY_H

#include "../chess_logic.h"

class EvaluationStrategy {

public:
    virtual ~EvaluationStrategy() = default;

    // Evaluate the board position and return a score
    virtual int evaluate(const ChessLogic::chessPiece* board) = 0;

};

#endif