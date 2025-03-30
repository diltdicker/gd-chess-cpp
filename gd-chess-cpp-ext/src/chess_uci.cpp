#include "chess_uci.h"
#include <new>
#include <cstdio>

extern "C" {

    // exported functions with C linkage that can be called from other languages
    void * ChessUci_Create() EXPORT_SYMBOL;
    void ChessUci_Destroy(void * instance) EXPORT_SYMBOL;
    const char * ChessUci_Command(void * uci_instance, const char * command) EXPORT_SYMBOL;

    void * ChessUci_Create() {
        return new(std::nothrow) ChessUCI;
    }

    void ChessUci_Destroy(void * instance) {
        delete static_cast<ChessUCI *>(instance);
    }

    const char * ChessUci_Command(void * uci_instance, const char * command) {
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
void ChessUCI::testUciClass() {
    printf("num calls: %d\n", numCalls);
    numCalls++;
    printf("Test UCI Class is instantiated\n");
}

char * ChessUCI::handleUciCommand(const char * command) {
    // Handle the UCI command and return a response
    // For demonstration, just echo the command back
    printf("Handling UCI command: %s\n", command);
    return const_cast<char *>(command);
}