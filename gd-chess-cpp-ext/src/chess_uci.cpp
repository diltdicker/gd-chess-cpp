#include "chess_uci.h"
#include <new>
#include <cstdio>
#include <regex>

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

char * ChessUCI::handleUciCommand(const char * command) {
    // Handle the UCI command and return a response
    // For demonstration, just echo the command back
    printf("Handling UCI command: %s\n", command);
    return const_cast<char *>(command);
}

void ChessUCI::fenBoardUpdate(const char * fen) {
    // Update the chess board based on the FEN string
    std::string fenStr(fen);
    std::regex fenRegex("([0-8prnbqkPRNBQK/]+) ([wb]) ([KQkq-]+) ([0-8a-h-]+) (\\d+) (\\d+)");
    std::smatch match;
    if (std::regex_match(fenStr, match, fenRegex)) {
        // Parse the FEN string
        std::string board = match[1].str();
        char turn = match[2].str()[0];
        std::string castling = match[3].str();
        std::string enPassant = match[4].str();
        int halfMove = std::stoi(match[5].str());
        int fullMove = std::stoi(match[6].str());

        // Update the chess board
        for (size_t i = 0; i < 64; i++) {
            chessBoard[i] = {0, 0}; // Initialize all squares to empty
        }
        
        size_t index = 0;
        for (char c : board) {
            if (isdigit(c)) {
                index += c - '0';
            } else {
                short color = (isupper(c)) ? WHITE_COLOR : BLACK_COLOR;
                short type = 0;
                switch (tolower(c)) {
                    case 'p': type = PAWN_TYPE; break;
                    case 'n': type = KNIGHT_TYPE; break;
                    case 'b': type = BISHOP_TYPE; break;
                    case 'r': type = ROOK_TYPE; break;
                    case 'q': type = QUEEN_TYPE; break;
                    case 'k': type = KING_TYPE; break;
                }
                chessBoard[index] = {color, type};
                index++;
            }
        }

        // Update other attributes
        isWhiteTurn = (turn == 'w');
        whiteQCastle = castling.find('Q') != std::string::npos;
        whiteKCastle = castling.find('K') != std::string::npos;
        blackQCastle = castling.find('q') != std::string::npos;
        blackKCastle = castling.find('k') != std::string::npos;
        
        if (enPassant != "-") {
            enPassantSquare = (enPassant[0] - 'a') + ((enPassant[1] - '1') * 8);
        } else {
            enPassantSquare = -1;
        }

        halfMoveClock = halfMove;
        fullMoveNumber = fullMove;

    } else {
        fprintf(stderr, "Invalid FEN string format. Aborting program.\n");
        std::abort(); // Crash the program intentionally
    }
    
}