#include "chess_bot.h"

// Evaluate the material score of the board
int ChessBot::evaluate_material(const ChessLogic &logic) const {
    int materialScore = 0;
    const ChessLogic::chessPiece* board = logic.getChessBoard();
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

// Evaluate the positional score of the board
int ChessBot::evaluate_position(const ChessLogic &logic) const {
    int positionScore = 0;
    const ChessLogic::chessPiece* board = logic.getChessBoard();
    bool endgame = logic.isEndgame();

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

// Evaluate the overall board position
int ChessBot::evaluate_board_position(const ChessLogic &logic) const {
    return evaluate_material(logic) + evaluate_position(logic);
}