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
#include "chess_logic.h"

class ChessUCI {

public:

    

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
    

    ChessLogic::chessPiece chessBoard[64];
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