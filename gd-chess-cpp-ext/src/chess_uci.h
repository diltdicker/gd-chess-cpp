#pragma once

#ifdef _WIN32
    // Define the export symbol for Windows
    // needed for DLLs
    #define EXPORT_SYMBOL __declspec(dllexport)
#else
    // Define the export symbol for other platforms (Linux, macOS)
    // needed for shared libraries
    #define EXPORT_SYMBOL __attribute__((visibility("default")))
#endif

#ifndef CHESS_UCI_H
#define CHESS_UCI_H

#include <string>

class ChessUCI {

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

    // Constructor
    ChessUCI();

    // Destructor
    ~ChessUCI();

    char * handleUciCommand(const char * command);

    void fenBoardUpdate(const char * fen);

protected:

    const short PAWN_TYPE = 1;
    const short KNIGHT_TYPE = 2;
    const short BISHOP_TYPE = 3;
    const short ROOK_TYPE = 4;
    const short QUEEN_TYPE = 5;
    const short KING_TYPE = 6;
    const short WHITE_COLOR = 1;
    const short BLACK_COLOR = 2;
    const short EMPTY_SQUARE = 0;
    const std::string DEFUATL_FEN = "rnbqkb1r/pppppppp/8/8/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1";

    chessPiece chessBoard[64];
    bool isWhiteTurn = true;
    bool whiteQCastle = true;
    bool whiteKCastle = true;
    bool blackQCastle = true;
    bool blackKCastle = true;
    int enPassantSquare = -1;
    int halfMoveClock = 0;
    int fullMoveNumber = 1;

private:


};

#endif