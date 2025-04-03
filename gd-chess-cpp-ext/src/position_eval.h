#ifndef POSITION_EVAL_H
#define POSITION_EVAL_H

#include "chess_logic.h"
#include "eval_strategy.h"

class PositionEvalStrategy : public EvaluationStrategy {

public:
    PositionEvalStrategy() = default;

    // Evaluate the board position and return a score
    int evaluate(const ChessLogic::chessPiece* board) override;

    bool isEndgame(const ChessLogic::chessPiece* board);

protected:

    const short WHT_PAWN_POS_TABLE[64] = {
        20, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 20, 20, 20, 20, 20,
        10, 10, 10, 10, 10, 10, 10, 10,
        0, 0, 0, 0, 0, 0, 0, 0,
    };
    const short BLK_PWN_POS_TABLE[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        10, 10, 10, 10, 10, 10, 10, 10,
        20, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 20, 20, 20, 20, 20,
    };

    const short WHT_PAWN_ENDGAME_TABLE[64] = {
        100, 100, 100, 100, 100, 100, 100, 100,
        80,  80,  80,  80,  80,  80,  80,  80,
        60,  60,  60,  60,  60,  60,  60,  60,
        40,  40,  40,  40,  40,  40,  40,  40,
        20,  20,  20,  20,  20,  20,  20,  20,
        10,  10,  10,  10,  10,  10,  10,  10,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0
    };

    const short BLK_PWN_ENDGAME_TABLE[64] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        10,  10,  10,  10,  10,  10,  10,  10,
        20,  20,  20,  20,  20,  20,  20,  20,
        40,  40,  40,  40,  40,  40,  40,  40,
        60,  60,  60,  60,  60,  60,  60,  60,
        80,  80,  80,  80,  80,  80,  80,  80,
        100, 100, 100, 100, 100, 100, 100, 100
    };

    const short WHT_KING_POS_TABLE[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        10, 10, 10, 10, 10, 10, 10, 10,
    };

    const short BLK_KING_POS_TABLE[64] = {
        10, 10, 10, 10, 10, 10, 10, 10,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
    };

    const short KING_ENDGAME_TABLE[64] = {
        0,  0,  0,  10, 10,  0,  0,  0,
        0,  10, 20, 30, 30, 20, 10,  0,
        0,  20, 40, 45, 45, 40, 20,  0,
        10, 30, 45, 50, 50, 45, 30, 10,
        10, 30, 45, 50, 50, 45, 30, 10,
        0,  20, 40, 45, 45, 40, 20,  0,
        0,  10, 20, 30, 30, 20, 10,  0,
        0,  0,  0,  10, 10,  0,  0,  0
    };

    const short KNIGHT_POS_TABLE[64] = {
        0,  0,  0,  10, 10,  0,  0,  0,
        0,  10, 20, 30, 30, 20, 10,  0,
        0,  20, 40, 45, 45, 40, 20,  0,
        10, 30, 45, 50, 50, 45, 30, 10,
        10, 30, 45, 50, 50, 45, 30, 10,
        0,  20, 40, 45, 45, 40, 20,  0,
        0,  10, 20, 30, 30, 20, 10,  0,
        0,  0,  0,  10, 10,  0,  0,  0
    };

    const short BISHOP_POS_TABLE[64] = {
        50, 40, 30, 20, 20, 30, 40, 50,
        40, 30, 25, 20, 20, 25, 30, 40,
        30, 25, 20, 15, 15, 20, 25, 30,
        20, 20, 15, 10, 10, 15, 20, 20,
        20, 20, 15, 10, 10, 15, 20, 20,
        30, 25, 20, 15, 15, 20, 25, 30,
        40, 30, 25, 20, 20, 25, 30, 40,
        50, 40, 30, 20, 20, 30, 40, 50
    };

    const short ROOK_POS_TABLE[64] = {
        30, 30, 30, 30, 30, 30, 30, 30,
        30, 30, 30, 30, 30, 30, 30, 30,
        10,  0,  0,  0,  0,  0,  0, 10,
        10,  0,  0,  0,  0,  0,  0, 10,
        10,  0,  0,  0,  0,  0,  0, 10,
        10,  0,  0,  0,  0,  0,  0, 10,
        30, 30, 30, 30, 30, 30, 30, 30,
        30, 30, 30, 30, 30, 30, 30, 30
    };

    const short QUEEN_POS_TABLE[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };

};

#endif