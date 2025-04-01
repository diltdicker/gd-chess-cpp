#include "material_eval.h"


int MaterialEvalStrategy::evaluate(const ChessLogic::chessPiece* board) {
    int materialScore = 0;

    for (int i = 0; i < 64; ++i) {
        const auto &piece = board[i];
        if (piece.color == 1) { // White
            materialScore += this->pieceValues[piece.type];
        } else if (piece.color == 2) { // Black
            materialScore -= this->pieceValues[piece.type];
        }
    }

    return materialScore;
}