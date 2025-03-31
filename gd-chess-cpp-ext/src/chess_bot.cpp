#include "chess_bot.h"
#include <regex>
#include <cstdio>
#include <cstdlib>

// Constructor that defaults the move strategy to RandomMoveStrategy
ChessBot::ChessBot() {
    moveStrategy = new RandomMoveStrategy();
    this->boardFromFEN(DEFAULT_FEN);
}

// Destructor that ensures the move strategy is deleted
ChessBot::~ChessBot() {
    if (moveStrategy != nullptr) {
        delete moveStrategy;
        moveStrategy = nullptr;
    }
}

void ChessBot::boardFromFEN(const std::string &fen) {
    std::regex fenRegex("([0-8prnbqkPRNBQK/]+) ([wb]) ([KQkq-]+) ([0-8a-h-]+) (\\d+) (\\d+)");
    std::smatch match;
    if (std::regex_match(fen, match, fenRegex)) {
        std::string board = match[1].str();
        char turn = match[2].str()[0];
        std::string castling = match[3].str();
        std::string enPassant = match[4].str();
        int halfMove = std::stoi(match[5].str());
        int fullMove = std::stoi(match[6].str());

        for (size_t i = 0; i < 64; i++) {
            chessBoard[i] = {0, 0};
        }

        size_t index = 0;
        for (char c : board) {
            if (isdigit(c)) {
                index += c - '0';
            } else {
                short color = (isupper(c)) ? 1 : 2;
                short type = 0;
                switch (tolower(c)) {
                    case 'p': type = 1; break;
                    case 'n': type = 2; break;
                    case 'b': type = 3; break;
                    case 'r': type = 4; break;
                    case 'q': type = 5; break;
                    case 'k': type = 6; break;
                }
                chessBoard[index] = {color, type};
                index++;
            }
        }

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

        // Update the bot logic with the new board state
        botLogic.copyChessBoard(chessBoard);
        botLogic.emtpyMoveStack();

    } else {
        fprintf(stderr, "Invalid FEN string format. Aborting program.\n");
        std::abort();
    }
}
