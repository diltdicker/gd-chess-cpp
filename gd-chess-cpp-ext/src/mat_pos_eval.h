#ifndef MAT_POS_EVAL_H
#define MAT_POS_EVAL_H

#include "chess_logic.h"
#include "eval_strategy.h"
#include "material_eval.h"
#include "position_eval.h"

class MatPosEvalStrategy : public EvaluationStrategy {
public:
    MatPosEvalStrategy() {
        materialEvalStrategy = new MaterialEvalStrategy();
        positionEvalStrategy = new PositionEvalStrategy();
    };

    ~MatPosEvalStrategy() {
        delete materialEvalStrategy;
        delete positionEvalStrategy;
    };

    // Evaluate the board position and return a score
    int evaluate(const ChessLogic::chessPiece* board) override {
        return materialEvalStrategy->evaluate(board) + positionEvalStrategy->evaluate(board);
    }

protected:

    MaterialEvalStrategy* materialEvalStrategy = nullptr;
    PositionEvalStrategy* positionEvalStrategy = nullptr;
};

#endif