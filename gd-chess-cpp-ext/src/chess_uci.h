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
#include "chess_bot.h"

class ChessUCI {

public:

    

    // Constructor
    ChessUCI();

    // Destructor
    ~ChessUCI();

    char * handleUciCommand(const char * command);

    void fenBoardUpdate(const char * fen);

    void setFENBoard(const char * fen);

    bool validateMove(const char * move);

    char * getBotMove(short searchDepth, int timeLimit);

    void setOption(const char * option, const char * value);

    char * getEval();

protected:



private:



};

#endif