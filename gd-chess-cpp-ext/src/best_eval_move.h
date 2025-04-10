#ifndef BEST_EVAL_MOVE_H
#define BEST_EVAL_MOVE_H

#include <vector>
#include <chrono>
#include <limits>
// #include <iostream>
#include <random>
#include <thread>
#include <mutex>
#include "chess_logic.h"
#include "move_strategy.h"

#ifdef DEBUG
#define DEBUG_PRINT(x) std::cout << "Debug: " << x << "\n";
#else
#define DEBUG_PRINT(x)
#endif


class BestEvalMoveStrategy : public MoveStrategy {
public:
    BestEvalMoveStrategy() = default;

    ChessLogic::evalMove getBestMove(ChessLogic &logic, EvaluationStrategy* evalStrategy,
        bool isWhite, short searchDepth, 
        std::chrono::time_point<std::chrono::steady_clock> stopTime) override;

    ChessLogic::evalMove getBestMove(ChessLogic &logic, EvaluationStrategy* evalStrategy,
        bool isWhite, short searchDepth, short threadCount,
        std::chrono::time_point<std::chrono::steady_clock> stopTime) override;     
        
    void getBestMoveThreaded(ChessLogic &logicBoard, EvaluationStrategy* evalStrategy,
        bool isWhite, std::stack<short> &depthstack, std::vector<ChessLogic::evalMove> &bestMove, std::mutex &mtx, 
        short &lastDepth, std::chrono::time_point<std::chrono::steady_clock> stopTime) override;     
    
protected:

int betaAlphaMinimax(ChessLogic * logic, int beta, int alpha, EvaluationStrategy* evalStrategy, bool isWhite, 
    short depth, std::chrono::time_point<std::chrono::steady_clock> stopTime);

void threadedSearch(ChessLogic &logic, const std::vector<ChessLogic::Move> searchMoves, 
    std::vector<ChessLogic::Move> &bestMoves, int &bestScore, std::mutex &mtx,  EvaluationStrategy* evalStrategy, 
    bool isWhite, short searchDepth, std::chrono::time_point<std::chrono::steady_clock> stopTime);

};

static void threadedTest(BestEvalMoveStrategy * moveStrategy, ChessLogic &logic, const std::vector<ChessLogic::Move> searchMoves);

#endif