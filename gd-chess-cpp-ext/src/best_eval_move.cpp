#include "best_eval_move.h"
#include <chrono>
#include <limits>

ChessLogic::Move BestEvalMoveStrategy::getBestMove(ChessLogic &logic, EvaluationStrategy* evalStrategy,
                                                   bool isWhite, short searchDepth, 
                                                   std::chrono::time_point<std::chrono::steady_clock> stopTime) {
    std::vector<ChessLogic::Move> legalMoves = logic.getLegalMoves(isWhite);
    if (legalMoves.empty()) {
        return ChessLogic::Move(); // Return a null move if no legal moves are available
    }

    ChessLogic::Move bestMove;
    int bestScore = std::numeric_limits<int>::min();

    for (const auto &move : legalMoves) {
        logic.makeMove(move);

        // Perform recursive search
        int score = -minimax(logic, evalStrategy, !isWhite, searchDepth - 1, stopTime);

        logic.undoMove();

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }

        // Check if the time limit has been reached
        if (std::chrono::steady_clock::now() >= stopTime) {
            break; // Exit early if the time limit is exceeded
        }
    }

    return bestMove;
}

int BestEvalMoveStrategy::minimax(ChessLogic &logic, EvaluationStrategy* evalStrategy, bool isWhite, 
                                  short depth, std::chrono::time_point<std::chrono::steady_clock> stopTime) {
    // Check if the time limit has been reached
    if (std::chrono::steady_clock::now() >= stopTime) {
        return 0; // Return a neutral score if the time limit is exceeded
    }

    // Base case: if depth is 0 or no legal moves are available
    if (depth == 0) {
        return evalStrategy->evaluate(&logic, isWhite) * (isWhite ? 1 : -1);
    }

    // Generate a hash for the current position
    uint64_t positionHash = logic.hashPosition(isWhite);

    // Check the transposition table for the current position
    if (logic.transpositionTable.find(positionHash) != logic.transpositionTable.end()) {
        return logic.transpositionTable[positionHash]; // Return the cached score
    }

    std::vector<ChessLogic::Move> legalMoves = logic.getLegalMoves(isWhite);
    if (legalMoves.empty()) {
        return evalStrategy->evaluate(&logic, isWhite) * (isWhite ? 1 : -1);
    }

    
    int bestScore = std::numeric_limits<int>::min();

    for (const auto &move : legalMoves) {
        logic.makeMove(move);

        // Recursive call to minimax
        int score = -minimax(logic, evalStrategy, !isWhite, depth - 1, stopTime);

        logic.undoMove();

        bestScore = std::max(bestScore, score);

        // Check if the time limit has been reached
        if (std::chrono::steady_clock::now() >= stopTime) {
            break; // Exit early if the time limit is exceeded
        }
    }

    logic.transpositionTable[positionHash] = bestScore; // Store the score in the transposition table

    return bestScore;
}