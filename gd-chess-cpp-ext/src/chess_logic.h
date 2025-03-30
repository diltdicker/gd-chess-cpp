#ifndef CHESS_LOGIC_H
#define CHESS_LOGIC_H

#include <vector>
#include <string>
#include <stack>
#include "chess_uci.h"
#include <limits>

class ChessLogic {
public:
    // Represents a move in chess (e.g., "e2e4")
    struct Move {
        short from; // Starting square (0-63)
        short to;   // Ending square (0-63)
        short promotion; // Promotion piece type (0-6 for none, knight, bishop, rook, queen, king)
        short capture; // Capture piece type (0-6 for none, knight, bishop, rook, queen, king)
        short color; // Color of the piece making the move (1 for white, 2 for black)
        short piece; // Piece type (1-6 for pawn to king)
        short moveType; // Type of move (0 for normal, 1 for Kindgside castling, 2 for Queenside castling, 3 for en passant)
    };

    // Constructor
    ChessLogic();

    // Destructor
    ~ChessLogic();

    // Get all legal moves for the current board position
    std::vector<Move> get_legal_moves(bool isWhite);

    // Evaluate the current board position
    // Returns a score where positive values favor white and negative values favor black
    int evaluate_board_position() const;

    void copyChessBoard(const ChessUCI::chessPiece inputBoard[64]);

    // Get the current chessboard
    const ChessUCI::chessPiece* getChessBoard() const;

    // Make a move on the board
    void make_move(const Move &move);

    void emtpyMoveStack();

    bool isInCheck(short color) const;

    // Undo the last move
    void undo_move();

    Move translateMove(short fromSquare, short toSquare) const;

    Move translateMove(const std::string &moveStr) const;

    short getSqureTopLeft(short square) const;
    short getSqureTopRight(short square) const;
    short getSqureBottomLeft(short square) const;
    short getSqureBottomRight(short square) const;
    short getSqureLeft(short square) const;
    short getSqureRight(short square) const;
    short getSqureTop(short square) const;
    short getSqureBottom(short square) const;
    short getSqureXYrelative(short square, short x, short y) const;

    // Helper methods for move generation and evaluation
    bool isMovePsuedoLegal(const Move &move) const;
    int evaluate_material() const;
    int evaluate_position() const;

    u_int64_t getPieceBitBoard(short color, short piece) const;
    u_int64_t getPieceBitBoard(short color) const;

    u_short getMinorPieceCnt(short color) const;
    u_short getMajorPieceCnt(short color) const;
    bool isEndgame() const;

protected:

    const u_short PAWN_VALUE = 100;
    const u_short KNIGHT_VALUE = 320;
    const u_short BISHOP_VALUE = 330;
    const u_short ROOK_VALUE = 500;
    const u_short KING_VALUE = 20000;
    const u_short QUEEN_VALUE = 900;

    const u_short pieceValues[7] = {
        0, // Empty
        PAWN_VALUE,
        KNIGHT_VALUE,
        BISHOP_VALUE,
        ROOK_VALUE,
        QUEEN_VALUE,
        KING_VALUE
    };

    ChessUCI::chessPiece internalBoard[64]; // 8x8 chess board represented as an array of pieces

    std::stack<Move> moveStack; // Stack to keep track of moves for undo functionality

    const u_short WHT_PAWN_POS_TABLE[64] = {
        20, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 20, 20, 20, 20, 20,
        10, 10, 10, 10, 10, 10, 10, 10,
        0, 0, 0, 0, 0, 0, 0, 0,
    };
    const u_short BLK_PWN_POS_TABLE[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        10, 10, 10, 10, 10, 10, 10, 10,
        20, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 20, 20, 20, 20, 20,
    };

    const u_short WHT_PAWN_ENDGAME_TABLE[64] = {
        100, 100, 100, 100, 100, 100, 100, 100, // First row
        80,  80,  80,  80,  80,  80,  80,  80,  // Second row
        60,  60,  60,  60,  60,  60,  60,  60,  // Third row
        40,  40,  40,  40,  40,  40,  40,  40,  // Fourth row
        20,  20,  20,  20,  20,  20,  20,  20,  // Fifth row
        10,  10,  10,  10,  10,  10,  10,  10,  // Sixth row
        0,   0,   0,   0,   0,   0,   0,   0,   // Seventh row
        0,   0,   0,   0,   0,   0,   0,   0    // Eighth row
    };

    const u_short BLK_PWN_ENDGAME_TABLE[64] = {
        0,   0,   0,   0,   0,   0,   0,   0,   // First row
        0,   0,   0,   0,   0,   0,   0,   0,   // Second row
        10,  10,  10,  10,  10,  10,  10,  10,  // Third row
        20,  20,  20,  20,  20,  20,  20,  20,  // Fourth row
        40,  40,  40,  40,  40,  40,  40,  40,  // Fifth row
        60,  60,  60,  60,  60,  60,  60,  60,  // Sixth row
        80,  80,  80,  80,  80,  80,  80,  80,  // Seventh row
        100, 100, 100, 100, 100, 100, 100, 100  // Eighth row
    };

    const u_short WHT_KING_POS_TABLE[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        10, 10, 10, 10, 10, 10, 10, 10,
    };

    const u_short BLK_KING_POS_TABLE[64] = {
        10, 10, 10, 10, 10, 10, 10, 10, // First row
        0,  0,  0,  0,  0,  0,  0,  0,  // Second row
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
    };


    const u_short KING_ENDGAME_TABLE[64] = {
        0,  0,  0,  10, 10,  0,  0,  0,
        0,  10, 20, 30, 30, 20, 10,  0,
        0,  20, 40, 45, 45, 40, 20,  0,
        10, 30, 45, 50, 50, 45, 30, 10,
        10, 30, 45, 50, 50, 45, 30, 10,
        0,  20, 40, 45, 45, 40, 20,  0,
        0,  10, 20, 30, 30, 20, 10,  0,
        0,  0,  0,  10, 10,  0,  0,  0
    };

    const u_short KNIGHT_POS_TABLE[64] = {
        0,  0,  0,  10, 10,  0,  0,  0,
        0,  10, 20, 30, 30, 20, 10,  0,
        0,  20, 40, 45, 45, 40, 20,  0,
        10, 30, 45, 50, 50, 45, 30, 10,
        10, 30, 45, 50, 50, 45, 30, 10,
        0,  20, 40, 45, 45, 40, 20,  0,
        0,  10, 20, 30, 30, 20, 10,  0,
        0,  0,  0,  10, 10,  0,  0,  0
    };

    const u_short BISHOP_POS_TABLE[64] = {
        50, 40, 30, 20, 20, 30, 40, 50, // First row
        40, 30, 25, 20, 20, 25, 30, 40, // Second row
        30, 25, 20, 15, 15, 20, 25, 30, // Third row
        20, 20, 15, 10, 10, 15, 20, 20, // Fourth row
        20, 20, 15, 10, 10, 15, 20, 20, // Fifth row
        30, 25, 20, 15, 15, 20, 25, 30, // Sixth row
        40, 30, 25, 20, 20, 25, 30, 40, // Seventh row
        50, 40, 30, 20, 20, 30, 40, 50  // Eighth row
    };

    const u_short ROOK_POS_TABLE[64] = {
        30, 30, 30, 30, 30, 30, 30, 30, // First row
        30, 30, 30, 30, 30, 30, 30, 30, // Second row
        10,  0,  0,  0,  0,  0,  0, 10, // Third row
        10,  0,  0,  0,  0,  0,  0, 10, // Fourth row
        10,  0,  0,  0,  0,  0,  0, 10, // Fifth row
        10,  0,  0,  0,  0,  0,  0, 10, // Sixth row
        30, 30, 30, 30, 30, 30, 30, 30, // Seventh row
        30, 30, 30, 30, 30, 30, 30, 30  // Eighth row
    };

    const u_short QUEEN_POS_TABLE[64] = {
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

#endif // CHESS_LOGIC_H