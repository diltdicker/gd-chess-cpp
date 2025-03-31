#ifndef CHESS_LOGIC_H
#define CHESS_LOGIC_H

#include <vector>
#include <string>
#include <stack>
#include <limits>

class ChessLogic {
public:

struct chessPiece
{
    short color; // 1 = white, 2 = black
    short type; // 1 = pawn, 2 = knight, 3 = bishop, 4 = rook, 5 = queen, 6 = king  

    // Overload the assignment operator
    chessPiece& operator=(const chessPiece& other) {
        if (this != &other) {
            this->color = other.color;
            this->type = other.type;
        }
        return *this;
    }
};

    // Represents a move in chess (e.g., "e2e4")
    struct Move {
        short from; // Starting square (0-63)
        short to;   // Ending square (0-63)
        short promotion; // Promotion piece type (0-6 for none, knight, bishop, rook, queen, king)
        short capture; // Capture piece type (0-6 for none, knight, bishop, rook, queen, king)
        short color; // Color of the piece making the move (1 for white, 2 for black)
        short piece; // Piece type (1-6 for pawn to king)
        short moveType; // Type of move (0 for normal, 1 for Kindgside castling, 2 for Queenside castling, 3 for en passant, 4 for pawn double move)

        // Default constructor for a null move
        Move() 
            : from(-1), to(-1), promotion(-1), capture(-1), color(-1), piece(-1), moveType(-1) {}
    };

    bool whiteQCastle = true;
    bool whiteKCastle = true;
    bool blackQCastle = true;
    bool blackKCastle = true;
    int enPassantSquare = -1;

    // Constructor
    ChessLogic();

    // Destructor
    ~ChessLogic();

    // Get all legal moves for the current board position
    std::vector<Move> get_legal_moves(bool isWhite);

    bool isMoveLegal(const Move &move);

    void copyChessBoard(const chessPiece inputBoard[64]);

    // Get the current chessboard
    const chessPiece* getChessBoard() const;

    // Make a move on the board
    void make_move(const Move &move);

    void emtpyMoveStack();

    bool isInCheck(short color) const;

    // Undo the last move
    void undo_move();

    Move translateMove(short fromSquare, short toSquare) const;

    Move translateMove(const std::string &moveStr) const;

    std::string translateMoveToString(const Move &move) const;

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

    u_int64_t getPieceBitBoard(short color, short piece) const;
    u_int64_t getPieceBitBoard(short color) const;
    u_int64_t getPawnMoveBitBoard(short color) const;
    u_int64_t getKnightMoveBitBoard(short color) const;
    u_int64_t getBishopMoveBitBoard(short color) const;
    u_int64_t getRookMoveBitBoard(short color) const;
    u_int64_t getQueenMoveBitBoard(short color) const;
    u_int64_t getKingMoveBitBoard(short color) const;

    std::vector<short> bitboardToSquares(u_int64_t bitboard) const;

protected:

    chessPiece internalBoard[64]; // 8x8 chess board represented as an array of pieces

    std::stack<Move> moveStack; // Stack to keep track of moves for undo functionality
    
};

#endif // CHESS_LOGIC_H