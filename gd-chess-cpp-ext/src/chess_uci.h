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

extern "C" {
    // exported functions with C linkage that can be called from other languages
    void * createChessUci();
    void destroyChessUci(void * instance);
    const char * inputCommand(void * uci_instance, const char * command);

    const char * inputFEN(void * uci_instance, const char * fen);
    const char * getFEN(void * uci_instance);

    const char * getBotMove(void * uci_instance, short searchDepth, int timeLimit);

    const char * validateMove(void * uci_instance, const char * move);

    void setOption(void * uci_instance, const char * option, const char * value);
}

class ChessUCI {

public:

    

    // Constructor
    ChessUCI();

    // Destructor
    ~ChessUCI();

    char * handleUciCommand(const char * command);

    void fenBoardUpdate(const char * fen);

    void importFEN(const char * fen);
    
    char * exportFEN();

    bool validateMove(const char * move);

    char * getBotMove(short searchDepth, int timeLimit);

    void setOption(const char * option, const char * value);

    char * getEval();

protected:



private:



};

#endif