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

    void inputFEN(void * uci_instance, const char * fen) {
        if (uci_instance) {
            static_cast<ChessUCI *>(uci_instance)->importFEN(fen);
        }
    }

    const char * exportFEN(void * uci_instance) {
        if (uci_instance) {
            return static_cast<ChessUCI *>(uci_instance)->exportFEN();
        }
        return nullptr;
    }

    const char * getBotMove(void * uci_instance, short searchDepth, int timeLimit) {
        if (uci_instance) {
            return static_cast<ChessUCI *>(uci_instance)->getBotMove(searchDepth, timeLimit);
        }
        return nullptr;
    }

    bool validateMove(void * uci_instance, const char * move) {
        if (uci_instance) {
            return static_cast<ChessUCI *>(uci_instance)->validateMove(move);
        }
        return false;
    }

    void setOption(void * uci_instance, const char * option, const char * value) {
        if (uci_instance) {
            static_cast<ChessUCI *>(uci_instance)->setOption(option, value);
        }
    }

    const char * debugBot(void * uci_instance, const char * option) {
        if (uci_instance) {
            return static_cast<ChessUCI *>(uci_instance)->debugBot(option);
        }
        return nullptr;
    }

    void makeMove(void * uci_instance, const char * move) {
        if (uci_instance) {
            static_cast<ChessUCI *>(uci_instance)->makeMove(move);
        }
    }

    char ** getMoveHistoryPtr(void * uci_instance) {
        if (uci_instance) {
            return static_cast<ChessUCI *>(uci_instance)->getMoveHistory();
        }
        return nullptr;
    }

    void freeMoveHistoryPtr(void * uci_instance, char **moveHistory) {
        if (uci_instance) {
            static_cast<ChessUCI *>(uci_instance)->freeMoveHistory(moveHistory);
        }
    }

    short getGameResult(void * uci_instance) {
        if (uci_instance) {
            return static_cast<ChessUCI *>(uci_instance)->getGameResult();
        }
        return 0;
    }
}

ChessUCI::ChessUCI() {
    // Constructor implementation
    chessBot = new(std::nothrow) ChessBot();
}
ChessUCI::~ChessUCI() {
    // Destructor implementation
    if (chessBot) {
        delete chessBot;
        chessBot = nullptr;
    }
}

char * ChessUCI::handleUciCommand(const char * command) {
    // Handle the UCI command and return a response
    // For demonstration, just echo the command back
    printf("Handling UCI command: %s\n", command);
    return const_cast<char *>(command);
}

void ChessUCI::importFEN(const char * fen) {
    // Import the FEN string into the chess bot
    if (chessBot) {
        chessBot->setFEN(fen);
    }
}

char * ChessUCI::exportFEN() {
    // Export the current FEN string from the chess bot
    if (chessBot) {
        return chessBot ? const_cast<char *>(chessBot->getFEN().c_str()) : nullptr;
    }
    return nullptr;
}

bool ChessUCI::validateMove(const char * move) {
    // Validate the move using the chess bot
    if (chessBot) {
        return chessBot->validateMove(move);
    }
    return false;
}

void ChessUCI::makeMove(const char * move) {
    // Make the move using the chess bot
    if (chessBot) {
        chessBot->applyMove(move);
    }
}

char * ChessUCI::getBotMove(short searchDepth, int timeLimit) {
    // Get the best move from the chess bot
    if (chessBot) {
        return const_cast<char *>(chessBot->getBestMove(searchDepth, timeLimit).c_str());
    }
    return nullptr;
}

void ChessUCI::setOption(const char * option, const char * value) {
    // Set options for the chess bot
    if (chessBot) {
        if (strcmp(option, "move_strategy") == 0) {
            chessBot->setMoveStrategy(value);
        } else if (strcmp(option, "eval_strategy") == 0) {
            chessBot->setEvalStrategy(value);
        } else {
            printf("Unknown option: %s\n", option);
        }
    }
}


char ** ChessUCI::getMoveHistory() {
    if (chessBot) {
        // Get the move history as a vector of strings
        std::vector<std::string> moveHistory = chessBot->translateMoveHistory();

        // Allocate memory for the array of C-style strings
        char **cStrings = (char **)malloc((moveHistory.size() + 1) * sizeof(char *));
        if (!cStrings) {
            return nullptr; // Return null if memory allocation fails
        }

        // Allocate memory for each string and copy the content
        for (size_t i = 0; i < moveHistory.size(); ++i) {
            cStrings[i] = (char *)malloc((moveHistory[i].size() + 1) * sizeof(char));
            if (!cStrings[i]) {
                // Free already allocated memory if allocation fails
                for (size_t j = 0; j < i; ++j) {
                    free(cStrings[j]);
                }
                free(cStrings);
                return nullptr;
            }
            std::strcpy(cStrings[i], moveHistory[i].c_str());
        }

        // Null-terminate the array
        cStrings[moveHistory.size()] = nullptr;

        return cStrings;
    }
    return nullptr;
}

short ChessUCI::getGameResult() {
    if (chessBot) {
        if (chessBot->getCheckmate() == 1) {
            return 2; // White wins
        } else if (chessBot->getCheckmate() == 2) {
            return 3; // Black wins
        } else if (chessBot->isStaleMate()) {
            return 4; // Stalemate
        } else if (chessBot->isCheck()) {
            return 1; // normal check
        }
    }
    return 0; // Game ongoing
}

void ChessUCI::freeMoveHistory(char **moveHistory) {
    if (moveHistory) {
        for (size_t i = 0; moveHistory[i] != nullptr; ++i) {
            free(moveHistory[i]); // Free each string
        }
        free(moveHistory); // Free the array
    }
}

char * ChessUCI::debugBot(const char * property) {
    if (chessBot) {
        static std::string joinedStrategies;
        const auto processStrategies = [&](const std::vector<std::string>& strategies) {
            joinedStrategies = std::accumulate(
                std::next(strategies.begin()), strategies.end(), strategies[0],
                [](std::string a, const std::string& b) { return a + "," + b; });
            return const_cast<char *>(joinedStrategies.c_str());
        };

        if (strcmp(property, "moves") == 0) {
            // return const_cast<char *>(chessBot->getAvailableMoves().c_str());
            return const_cast<char *>("");
        } else if (strcmp(property, "color") == 0) {
            return const_cast<char *>(chessBot->whosTurn().c_str());
        } else if (strcmp(property, "list_move_strategy") == 0) {
            return processStrategies(chessBot->listMoveStrategies());
        } else if (strcmp(property, "list_eval_strategy") == 0) {
            return processStrategies(chessBot->listEvalStrategies());
        } else if (strcmp(property, "current_move_strategy") == 0) {
            return const_cast<char *>(chessBot->getCurrentMoveStrategy().c_str());
        } else if (strcmp(property, "current_eval_strategy") == 0) {
            return const_cast<char *>(chessBot->getCurrentEvalStrategy().c_str());
        } else {
            static std::string errorMsg = "Unknown property: ";
            errorMsg += property;
            return const_cast<char *>(errorMsg.c_str());
        }
    }
    return nullptr;
}