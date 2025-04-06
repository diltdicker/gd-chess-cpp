#ifndef CHESS_LOGIC_H
#define CHESS_LOGIC_H

#include <vector>
#include <string>
#include <stack>
#include <limits>
#include <cstdint>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <functional>
#include <random>
#include <bitset>

#ifdef DEBUG
#define DEBUG_PRINT(x) std::cout << "Debug: " << x <<  "\n";
#else
#define DEBUG_PRINT(x)
#endif

class ChessLogic {
public:

struct moveHash
{
    int score;
    short depth;
};

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

    chessPiece(const chessPiece& other) : color(other.color), type(other.type) {}

    // Constructor for chessPiece
    chessPiece(short color = 0, short type = 0) : color(color), type(type) {}
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
            : from(-1), to(-1), promotion(0), capture(0), color(0), piece(0), moveType(0) {}

        // Copy constructor (deep copy isn't needed here, just copying the fields)
        Move(const Move& other)
            : from(other.from), to(other.to), promotion(other.promotion),
                capture(other.capture), color(other.color), piece(other.piece), moveType(other.moveType) {}
    };

    // change to stack type TODO
    bool whiteQCastle = true
    bool whiteKCastle = true;
    bool blackQCastle = true;
    bool blackKCastle = true;
    int enPassantSquare = -1;

    // Constructor
    ChessLogic();

    // Destructor
    ~ChessLogic();

    // Get all legal moves for the current board position
    std::vector<Move> getLegalMoves(bool isWhite);

    std::string printLegalMoves(const std::vector<Move> moves) const;

    bool isMoveLegal(const Move &move);

    void copyChessBoard(const chessPiece inputBoard[64]);

    // Get the current chessboard
    const chessPiece* getChessBoard() const;

    // Make a move on the board
    void makeMove(const Move &move);

    void emtpyMoveStack();

    bool isInCheck(short color) const;

    // Undo the last move
    void undoMove();

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

    uint64_t getColorBitBoard(short color) const;
    uint64_t getPieceBitBoard(short color, short piece) const;
    uint64_t getPawnMoveBitBoard(short color) const;
    uint64_t getKnightMoveBitBoard(short color) const;
    uint64_t getBishopMoveBitBoard(short color) const;
    uint64_t getRookMoveBitBoard(short color) const;
    uint64_t getQueenMoveBitBoard(short color) const;
    uint64_t getKingMoveBitBoard(short color) const;

    std::vector<short> bitboardToSquares(uint64_t bitboard) const;

    std::string printBitBoard(uint64_t bitboard) const;

    std::string printBoard() const;

    uint64_t hashPosition(bool isWhiteTurn) const;

    void initializeZobrist();

    std::unordered_map<uint64_t, moveHash> transpositionTable;

    std::vector<Move> getMoveHistory() const;

    std::string printMoves(std::vector<Move> moves) const;

    std::string squareToString(short square) const;

    short stringToSquare(const std::string &squareStr) const;


protected:

    chessPiece internalBoard[64]; // 8x8 chess board represented as an array of pieces

    std::stack<Move> moveStack; // Stack to keep track of moves for undo functionality

private:
    uint64_t zobristTable[64][12]; // Random values for pieces on squares
    uint64_t zobristCastling[4];   // Random values for castling rights
    uint64_t zobristEnPassant[8];  // Random values for en passant files
    uint64_t zobristTurn;          // Random value for the player's turn


};


#endif // CHESS_LOGIC_H