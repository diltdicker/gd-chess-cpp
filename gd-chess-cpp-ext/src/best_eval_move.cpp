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

    ChessLogic::Move bestMove = ChessLogic::Move();
    int low = std::numeric_limits<int>::min();
    int high = std::numeric_limits<int>::max();
    int bestScore = isWhite ? low : high;

    for (const auto &move : legalMoves) {
        logic.makeMove(move);

        if (logic.isInCheck(!isWhite) && logic.getLegalMoves(!isWhite).size() == 0) { // Skip moves that put the king in check
            logic.undoMove();
            return move; // Return the move that puts the opponent in checkmate
        }

        // Perform recursive search
        int score = betaAlphaMinimax(logic, high, low, evalStrategy, !isWhite, searchDepth - 1, stopTime); // score will be positive for white, negative for black

        logic.undoMove();

        // DEBUG_PRINT("depth: " << searchDepth << " move: "  << logic.translateMoveToString(move) << " score: " << score);

        if (isWhite) {
            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
        } else {
            if (score < bestScore) {
                bestScore = score;
                bestMove = move;
            }
        }

        // Check if the time limit has been reached
        // if (std::chrono::steady_clock::now() >= stopTime) {
        //     DEBUG_PRINT("Time limit exit: getBestMove");
        //     break; // Exit early if the time limit is exceeded
        // }
    }

    DEBUG_PRINT("best move: " << logic.translateMoveToString(bestMove) << " score: "  << bestScore);

    return bestMove;
}


int BestEvalMoveStrategy::betaAlphaMinimax(ChessLogic &logic, int beta, int alpha, EvaluationStrategy* evalStrategy, bool isWhite, 
    short depth, std::chrono::time_point<std::chrono::steady_clock> stopTime) {
        DEBUG_PRINT("betaAplpha depth:" << depth);

        const std::vector<ChessLogic::Move> legalMoves = logic.getLegalMoves(isWhite);
        DEBUG_PRINT("BEFORE checkmate check");
        if (legalMoves.size() == 0) {
            if (logic.isInCheck(isWhite)) {
                return isWhite ? -30000 + (depth * -1000) : 30000 + (depth * 1000);
            }
            return 0; // stalemate
        }

        if (depth == 0) {
            DEBUG_PRINT("BEFORE EVAL");
            return evalStrategy->evaluate(&logic, isWhite);
        }

        // uint64_t positionHash = logic.hashPosition(isWhite);

        // if (logic.transpositionTable.find(positionHash) != logic.transpositionTable.end()) {
        //         // DEBUG_PRINT("position already reached...skiping");
        //         return logic.transpositionTable[positionHash]; // Return the cached score
        // }

        DEBUG_PRINT("BEFORE init bestscore");
        int bestScore = isWhite ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

        for (const auto &move : legalMoves) {

            logic.makeMove(move);
            
            DEBUG_PRINT("BEFORE init score");
            int score = betaAlphaMinimax(logic, beta, alpha, evalStrategy, !isWhite, depth - 1, stopTime);

            if (isWhite) {  // pick negative score for black & positive for white

                DEBUG_PRINT("BEFORE update bestscore white");
                bestScore = std::max(bestScore, score);
                alpha = std::max(alpha, score);
                if (beta <= alpha) {
                    logic.undoMove();
                    break;
                }

            } else {
                DEBUG_PRINT("BEFORE update bestscore black");
                bestScore = std::min(bestScore, score);
                beta = std::min(beta, score);
                if (alpha >= beta) {
                    logic.undoMove();
                    break;
                }
            }

            logic.undoMove();
        }
        DEBUG_PRINT("BEFORE return bestScore" << bestScore);
        return bestScore;
    }

std::string BestEvalMoveStrategy::printScores(std::vector<ChessLogic::evalMove> moves) {

    std::string scoreString;
    for (const auto &evalMove : moves) {
        scoreString += "[ ";
        scoreString += evalMove.move;
        scoreString += ": ";
        scoreString += std::to_string(evalMove.score);
        scoreString += "] ";
    }
    return scoreString;
}