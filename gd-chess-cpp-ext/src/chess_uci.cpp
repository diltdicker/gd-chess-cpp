#include "chess_uci.h"
#include <new>
#include <cstdio>
#include <regex>

extern "C" {

    // exported functions with C linkage that can be called from other languages

    void * createChessUci() {
        return new(std::nothrow) ChessUCI;
    }

    void destroyChessUci(void * instance) {
        delete static_cast<ChessUCI *>(instance);
    }

    const char * inputCommand(void * uci_instance, const char * command) {
        printf("Received command: %s\n", command);
        if (uci_instance) {
            return static_cast<ChessUCI *>(uci_instance)->handleUciCommand(command);
        } else {
            return "Invalid ChessUCI instance";
        }
    }

    
}

ChessUCI::ChessUCI() {
    // Constructor implementation
}
ChessUCI::~ChessUCI() {
    // Destructor implementation
}

char * ChessUCI::handleUciCommand(const char * command) {
    // Handle the UCI command and return a response
    // For demonstration, just echo the command back
    printf("Handling UCI command: %s\n", command);
    return const_cast<char *>(command);
}