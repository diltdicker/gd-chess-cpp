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

class ChessUCI {

private:
    int numCalls = 0;

public:
    // Constructor
    ChessUCI();

    // Destructor
    ~ChessUCI();

    void testUciClass();

    char * handleUciCommand(const char * command);

};

#endif