#ifndef CHESS_BOT_H
#define CHESS_BOT_H

#include <vector>
#include <iostream>
#include "chess_logic.h"
#include "strategy/move_strategy.h"
#include "strategy/eval_strategy.h"
#include "strategy/random_move.h"
#include "strategy/no_eval.h"


class ChessBot {

protected:

    const std::string RANDOM_STRATEGY = "random";
    const std::string NO_EVAL_STRATEGY = "no_eval";

    MoveStrategy * moveStrategy = nullptr;
    EvaluationStrategy * evalStrategy = nullptr;

public:
    const std::string DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    // Constructor
    ChessBot();

    // Destructor
    ~ChessBot();

    void applyMove(const std::string &move);

    ChessLogic::Move getBestMove(long timeLimit);

    void setMoveStrategy(const std::string &strategy) {
        if (moveStrategy != nullptr) {
            delete moveStrategy;
        }

        if (strategy == RANDOM_STRATEGY) {
            moveStrategy = new RandomMoveStrategy();
        } else {
            std::cerr << "Error: Invalid move strategy: " << strategy << std::endl;
            abort(); // Invalid strategy
        }
    }

    void setEvalStrategy(const std::string &strategy) {
        if (evalStrategy != nullptr) {
            delete evalStrategy;
        }

        if (strategy == NO_EVAL_STRATEGY) {
            evalStrategy = new NoEvalStrategy();
        } else {
            std::cerr << "Error: Invalid evaluation strategy: " << strategy << std::endl;
            abort(); // Invalid strategy
        }
    }

    std::vector<std::string> listMoveStrategies() {
        return { RANDOM_STRATEGY };
    }

    std::vector<std::string> listEvalStrategies() {
        return { NO_EVAL_STRATEGY};
    }

    void setFEN(const std::string &fen);
    
protected:

    bool isWhiteTurn = true;

    int halfMoveClock = 0;
    int fullMoveNumber = 1;

    bool isWhite = true;

    ChessLogic botLogic = ChessLogic();

};

#endif