#ifndef RANDOM_MOVE_H
#define RANDOM_MOVE_H

#include <vector>
#include "chess_logic.h"
#include "move_strategy.h"
#include "eval_strategy.h"

class RandomMoveStrategy : public MoveStrategy {

public:
    RandomMoveStrategy() = default;

    ChessLogic::evalMove getBestMove(ChessLogic &logic, EvaluationStrategy* evalStrategy,
                                 bool isWhite, short searchDepth, std::chrono::time_point<std::chrono::steady_clock> stopTime) override;

    ChessLogic::evalMove getBestMove(ChessLogic &logic, EvaluationStrategy* evalStrategy, 
        bool isWhite, short maxDepth, short threadCount, std::chrono::time_point<std::chrono::steady_clock> stopTime) override;

    void getBestMoveThreaded(ChessLogic &logicBoard, EvaluationStrategy* evalStrategy,
        bool isWhite, std::stack<short> &depthstack, std::vector<ChessLogic::evalMove> &bestMove, std::mutex &mtx, 
        short &lastDepth, std::chrono::time_point<std::chrono::steady_clock> stopTime) override {
            getBestMove(logicBoard, evalStrategy, isWhite, depthstack.size(), stopTime);
    }
};

#endif