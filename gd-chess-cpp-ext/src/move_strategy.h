#ifndef MOVE_STRATEGY_H
#define MOVE_STRATEGY_H

#include <chrono>
#include <mutex>
#include "chess_logic.h"
#include "eval_strategy.h"

class MoveStrategy {
    public:

        virtual ~MoveStrategy() = default;

        virtual ChessLogic::evalMove getBestMove(ChessLogic &logic, EvaluationStrategy* evalStrategy,
            bool isWhite, short maxDepth, std::chrono::time_point<std::chrono::steady_clock> stopTime) = 0; 

        virtual ChessLogic::evalMove getBestMove(ChessLogic &logic, EvaluationStrategy* evalStrategy, 
            bool isWhite, short maxDepth, short threadCount, std::chrono::time_point<std::chrono::steady_clock> stopTime) = 0;

        virtual void getBestMoveThreaded(ChessLogic &logicBoard, EvaluationStrategy* evalStrategy,
            bool isWhite, std::stack<short> &depthstack, std::vector<ChessLogic::evalMove> &bestMove, std::mutex &mtx, 
            short &lastDepth, std::chrono::time_point<std::chrono::steady_clock> stopTime) = 0; 
};

#endif