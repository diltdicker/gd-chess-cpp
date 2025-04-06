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

#ifdef DEBUG
#define DEBUG_PRINT(x) std::cout << "Debug: " << x <<  "\n";
#else
#define DEBUG_PRINT(x)
#endif

#include <string>
#include "chess_bot.h"

extern "C" {
    // exported functions with C linkage that can be called from other languages
    EXPORT_SYMBOL void * createChessUci();

    EXPORT_SYMBOL void destroyChessUci(void * instance);

    EXPORT_SYMBOL const char * inputCommand(void * uci_instance, const char * command);

    EXPORT_SYMBOL void inputFEN(void * uci_instance, const char * fen);

    EXPORT_SYMBOL const char * exportFEN(void * uci_instance);

    EXPORT_SYMBOL char ** getMoveHistoryPtr(void * uci_instance);

    EXPORT_SYMBOL void freeMoveHistoryPtr(void * uci_instance, char **moveHistory);

    EXPORT_SYMBOL const char * getBotMove(void * uci_instance, short searchDepth, int timeLimit);

    EXPORT_SYMBOL bool validateMove(void * uci_instance, const char * move);

    EXPORT_SYMBOL void makeMove(void * uci_instance, const char * move);

    EXPORT_SYMBOL void setOption(void * uci_instance, const char * option, const char * value);

    EXPORT_SYMBOL short getGameResult(void * uci_instance);

    EXPORT_SYMBOL const char * debugBot(void * uci_instance, const char * property);
}

class ChessUCI {

public:

    

    // Constructor
    ChessUCI();

    // Destructor
    ~ChessUCI();

    char * handleUciCommand(const char * command);

    void importFEN(const char * fen);
    
    char * exportFEN();

    bool validateMove(const char * move);

    void makeMove(const char * move);

    char * getBotMove(short searchDepth, int timeLimit);

    void setOption(const char * option, const char * value);

    char * getEval();

    char ** getMoveHistory();

    void freeMoveHistory(char **moveHistory);

    // returns 1 if a check, 2 if white wins, 3 if black wins, 4 if draw, 0 if no result
    short getGameResult();

    char * debugBot(const char * property);

protected:

    ChessBot * chessBot = nullptr;


private:



};

#endif