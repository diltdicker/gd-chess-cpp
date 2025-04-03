#ifndef MATERIAL_EVAL_H
#define MATERIAL_EVAL_H

#include "chess_logic.h"
#include "eval_strategy.h"

class MaterialEvalStrategy : public EvaluationStrategy {

public:
    MaterialEvalStrategy() = default;

    // Evaluate the board position and return a score
    int evaluate(const ChessLogic::chessPiece* board) override;

protected:

    const short PAWN_VALUE = 100;
    const short KNIGHT_VALUE = 320;
    const short BISHOP_VALUE = 330;
    const short ROOK_VALUE = 500;
    const short KING_VALUE = 20000;
    const short QUEEN_VALUE = 900;

    const short pieceValues[7] = {
        0, // Empty
        PAWN_VALUE,
        KNIGHT_VALUE,
        BISHOP_VALUE,
        ROOK_VALUE,
        QUEEN_VALUE,
        KING_VALUE
    };

};

#endif