#ifndef CHESS_BOT_H
#define CHESS_BOT_H

#include <vector>
#include "chess_logic.h"
#include "strategy/move_strategy.h"
#include "strategy/random_move.h"


class ChessBot {

protected:
    const std::string RANDOM_STRATEGY = "random";

    MoveStrategy * moveStrategy = nullptr;

public:
    const std::string DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    // Constructor
    ChessBot();

    // Destructor
    ~ChessBot();

    void setFEN(const std::string &fen);

    void applyMove(const std::string &move);

    ChessLogic::Move getBestMove(long timeLimit);

    void setMoveStrategy(const std::string &strategy) {
        if (moveStrategy != nullptr) {
            delete moveStrategy;
        }

        if (strategy == RANDOM_STRATEGY) {
            moveStrategy = new RandomMoveStrategy();
        } else {
            // Add other strategies here
        }
    }

    std::vector<std::string> listStrategies() {
        return { RANDOM_STRATEGY };
    }

    void boardFromFEN(const std::string &fen);

protected:

    ChessLogic::chessPiece chessBoard[64];
    bool isWhiteTurn = true;
    bool whiteQCastle = true;
    bool whiteKCastle = true;
    bool blackQCastle = true;
    bool blackKCastle = true;
    int enPassantSquare = -1;
    int halfMoveClock = 0;
    int fullMoveNumber = 1;

    bool isWhite = true;

    ChessLogic botLogic = ChessLogic();

};

#endif