#include "position_eval.h"


bool PositionEvalStrategy::isEndgame(const ChessLogic::chessPiece* board) {
    int majorPieces = 0; // Count queens and rooks
    int minorPieces = 0; // Count bishops and knights

    for (int i = 0; i < 64; ++i) {
        const auto &piece = board[i];
        if (piece.type == 0) continue; // Skip empty squares

        if (piece.type == 5 || piece.type == 4) { // Queen or Rook
            majorPieces++;
        } else if (piece.type == 3 || piece.type == 2) { // Bishop or Knight
            minorPieces++;
        }
    }

    // Endgame is defined as having few major and minor pieces left
    return (majorPieces <= 2 && minorPieces <= 2);
}

int PositionEvalStrategy::evaluate(const ChessLogic * chessBoard, bool whitesTurn) {
    int positionScore = 0;
    bool endgame = isEndgame(chessBoard->getChessBoard()); // Placeholder for endgame condition
    const auto *board = chessBoard->getChessBoard();

    for (int i = 0; i < 64; ++i) {
        const auto &piece = board[i];
        if (piece.type == 0) continue; // Skip empty squares

        switch (piece.type) {
            case 1: // Pawn
                if (endgame) {
                    positionScore += (piece.color == 1 ? this->WHT_PAWN_ENDGAME_TABLE[i] : -this->BLK_PWN_ENDGAME_TABLE[i]);
                } else {
                    positionScore += (piece.color == 1 ? this->WHT_PAWN_POS_TABLE[i] : -this->BLK_PWN_POS_TABLE[i]);
                }
                break;
            case 2: // Knight
                positionScore += (piece.color == 1 ? this->KNIGHT_POS_TABLE[i] : -this->KNIGHT_POS_TABLE[i]);
                break;
            case 3: // Bishop
                positionScore += (piece.color == 1 ? this->BISHOP_POS_TABLE[i] : -this->BISHOP_POS_TABLE[i]);
                break;
            case 4: // Rook
                positionScore += (piece.color == 1 ? this->ROOK_POS_TABLE[i] : -this->ROOK_POS_TABLE[i]);
                break;
            case 5: // Queen
                positionScore += (piece.color == 1 ? this->QUEEN_POS_TABLE[i] : -this->QUEEN_POS_TABLE[i]);
                break;
            case 6: // King
                if (endgame) {
                    positionScore += (piece.color == 1 ? this->KING_ENDGAME_TABLE[i] : -this->KING_ENDGAME_TABLE[i]);
                } else {
                    positionScore += (piece.color == 1 ? this->WHT_KING_POS_TABLE[i] : -this->BLK_KING_POS_TABLE[i]);
                }
                break;
        }
    }

    return positionScore;
}