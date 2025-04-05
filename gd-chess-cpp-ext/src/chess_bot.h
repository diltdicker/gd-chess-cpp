#ifndef CHESS_BOT_H
#define CHESS_BOT_H

#include <vector>
#include <chrono>
#include <iostream>
#include <regex>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "chess_logic.h"
#include "move_strategy.h"
#include "eval_strategy.h"
#include "random_move.h"
#include "no_eval.h"
#include "best_eval_move.h"
#include "material_eval.h"
#include "position_eval.h"
#include "mat_pos_eval.h"

#ifdef DEBUG
#define DEBUG_PRINT(x) std::cout << "Debug: " << x << std::endl
#else
#define DEBUG_PRINT(x)
#endif

class ChessBot
{

protected:
    // move strategies
    const std::string BEST_EVAL_MOVE_STRATEGY = "best_eval_move";
    const std::string RANDOM_STRATEGY = "random";

    // evaluation strategies
    const std::string POSITION_EVAL_STRATEGY = "position_eval";
    const std::string MATERIAL_EVAL_STRATEGY = "material_eval";
    const std::string MAT_POS_EVAL_STRATEGY = "mat_pos_eval";
    const std::string NO_EVAL_STRATEGY = "no_eval";

    MoveStrategy *moveStrategy = nullptr;
    EvaluationStrategy *evalStrategy = nullptr;

public:
    struct moveEvaluation
    {
        ChessLogic::Move move;
        int moveScore;
    };

    const std::string DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    // Constructor
    ChessBot();

    // Destructor
    ~ChessBot();

    void applyMove(const std::string &move);

    std::string getBestMove(short searchDepth, int timeLimit)
    {
        DEBUG_PRINT("Searching for best move...");
        DEBUG_PRINT(getAvailableMoves());
        DEBUG_PRINT("try this");
        std::chrono::time_point<std::chrono::steady_clock> stopTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeLimit);
        return botLogic.translateMoveToString(iterativeDeepeningSearch(searchDepth, stopTime));
    }

    ChessLogic::Move iterativeDeepeningSearch(short searchDepth, std::chrono::time_point<std::chrono::steady_clock> stopTime);

    bool validateMove(const std::string &move);

    void setMoveStrategy(const std::string &strategy)
    {
        if (moveStrategy != nullptr)
        {
            delete moveStrategy;
            currentMoveStrategy = "";
        }

        if (strategy == RANDOM_STRATEGY)
        {
            moveStrategy = new RandomMoveStrategy();
            currentMoveStrategy = RANDOM_STRATEGY;
        }
        else if (strategy == BEST_EVAL_MOVE_STRATEGY)
        {
            moveStrategy = new BestEvalMoveStrategy();
            currentMoveStrategy = BEST_EVAL_MOVE_STRATEGY;
        }
        else
        {
            std::cerr << "Error: Invalid move strategy: " << strategy << std::endl;
            abort(); // Invalid strategy
        }
    }

    void setEvalStrategy(const std::string &strategy)
    {
        if (evalStrategy != nullptr)
        {
            delete evalStrategy;
            currentEvalStrategy = "";
        }

        if (strategy == NO_EVAL_STRATEGY)
        {
            evalStrategy = new NoEvalStrategy();
        }
        else if (strategy == POSITION_EVAL_STRATEGY)
        {
            evalStrategy = new PositionEvalStrategy();
            currentEvalStrategy = POSITION_EVAL_STRATEGY;
        }
        else if (strategy == MATERIAL_EVAL_STRATEGY)
        {
            evalStrategy = new MaterialEvalStrategy();
            currentEvalStrategy = MATERIAL_EVAL_STRATEGY;
        }
        else if (strategy == MAT_POS_EVAL_STRATEGY)
        {
            evalStrategy = new MatPosEvalStrategy();
            currentEvalStrategy = MAT_POS_EVAL_STRATEGY;
        }
        else
        {
            std::cerr << "Error: Invalid evaluation strategy: " << strategy << std::endl;
            abort(); // Invalid strategy
        }
    }

    std::vector<std::string> listMoveStrategies()
    {
        return {RANDOM_STRATEGY, BEST_EVAL_MOVE_STRATEGY};
    }

    std::vector<std::string> listEvalStrategies()
    {
        return {NO_EVAL_STRATEGY, POSITION_EVAL_STRATEGY, MATERIAL_EVAL_STRATEGY, MAT_POS_EVAL_STRATEGY};
    }

    void setFEN(const std::string &fen);

    const std::string getFEN();

    bool isCheck() const;

    short getCheckmate()
    {
        if (botLogic.isInCheck(isWhiteTurn) && botLogic.getLegalMoves(isWhiteTurn).empty())
        {
            return isWhiteTurn ? 2 : 1; // 2 for black wins, 1 for white wins
        }
        return 0; // No checkmate
    }

    bool isStaleMate();

    std::vector<ChessLogic::Move> getMoveHistory() const;

    std::vector<std::string> translateMoveHistory() const;

    bool isThreefoldRepetition() const;

    std::string getCurrentMoveStrategy() const
    {
        return currentMoveStrategy;
    }

    std::string getCurrentEvalStrategy() const
    {
        return currentEvalStrategy;
    }

    std::string getAvailableMoves();

    const std::string whosTurn() const;

protected:
    bool isWhiteTurn = true;

    int halfMoveClock = 0;
    int fullMoveNumber = 1;

    std::string currentMoveStrategy;
    std::string currentEvalStrategy;
    ;

    ChessLogic botLogic;
    ;
};

#endif