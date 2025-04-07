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
        int score = betaAlphaMinimax(logic, low, high, evalStrategy, !isWhite, searchDepth - 1, stopTime); // score will be positive for white, negative for black

        logic.undoMove();

        DEBUG_PRINT("depth: " << searchDepth << " move: "  << logic.translateMoveToString(move) << " score: " << score);

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

// int BestEvalMoveStrategy::minimax(ChessLogic &logic, EvaluationStrategy* evalStrategy, bool isWhite, 
//                                   short depth, std::chrono::time_point<std::chrono::steady_clock> stopTime) {
//     // Check if the time limit has been reached
//     if (std::chrono::steady_clock::now() >= stopTime) {
//         return 0; // Return a neutral score if the time limit is exceeded
//     }

//     // Base case: if depth is 0 or no legal moves are available
//     if (depth == 0) {
//         return evalStrategy->evaluate(&logic, isWhite) * (isWhite ? 1 : -1);
//     }

//     // Generate a hash for the current position
//     uint64_t positionHash = logic.hashPosition(isWhite);

//     // Check the transposition table for the current position
//     // if (logic.transpositionTable.find(positionHash) != logic.transpositionTable.end()) {
//     //     DEBUG_PRINT("position already reached...skiping");
//     //     return logic.transpositionTable[positionHash]; // Return the cached score
//     // }

//     std::vector<ChessLogic::Move> legalMoves = logic.getLegalMoves(isWhite);
//     if (legalMoves.empty() && logic.isInCheck(isWhite)) {
//         DEBUG_PRINT("is a checkmate position");
//     }

//     if (legalMoves.empty()) {
//         // return evalStrategy->evaluate(&logic, isWhite) * (isWhite ? 1 : -1);
//         return std::numeric_limits<int>::min() * (isWhite ? 1 : -1);
//     }

    
//     int bestScore = std::numeric_limits<int>::min();

//     for (const auto &move : legalMoves) {
//         logic.makeMove(move);

        
//         // Recursive call to minimax
//         int score = -minimax(logic, evalStrategy, !isWhite, depth - 1, stopTime);
//         // DEBUG_PRINT("Evaluating move: " + logic.printMoves(logic.getMoveHistory()) << " score: " << score << " isWhite: " << isWhite);

//         logic.undoMove();
        
//         bestScore = std::max(bestScore, score);

//         // Check if the time limit has been reached
//         if (std::chrono::steady_clock::now() >= stopTime) {
//             DEBUG_PRINT("Time limit exit: minimax");
//             break; // Exit early if the time limit is exceeded
//         }
//     }

//     // if (depth == 1) {
//     //     logic.transpositionTable[positionHash] = {bestScore, depth}; // Store the score in the transposition table
//     // }

//     return bestScore;
// }

// int BestEvalMoveStrategy::minimax2(ChessLogic &logic, EvaluationStrategy* evalStrategy, bool isWhite, 
//     short depth, std::chrono::time_point<std::chrono::steady_clock> stopTime) {

//         std::vector<ChessLogic::Move> legalMoves = logic.getLegalMoves(isWhite);
        
//         if (legalMoves.size() == 0) {
//             if (logic.isInCheck(isWhite)) {
//                 return isWhite ? -30000 : 30000;
//                 // return isWhite ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max(); // checkmate
//             }
//             return 0; // stalemate
//         }

//         if (depth == 0) {
            
//             return evalStrategy->evaluate(&logic, isWhite);
//         }

//         uint64_t positionHash = logic.hashPosition(isWhite);

//         // if (logic.transpositionTable.find(positionHash) != logic.transpositionTable.end()) {
//         //         // DEBUG_PRINT("position already reached...skiping");
//         //         return logic.transpositionTable[positionHash]; // Return the cached score
//         // }

//         int bestScore = isWhite ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
//         for (ChessLogic::Move move : legalMoves) {
//             if (move.color == 0) {
//             }

//             logic.makeMove(move);
            
//             int score = minimax2(logic, evalStrategy, !isWhite, depth - 1, stopTime);
//             if (isWhite) {  // pick negative score for black & positive for white
//                 bestScore = std::max(bestScore, score);
//             } else {
//                 bestScore = std::min(bestScore, score);
//             }

//             logic.undoMove();
//         }
//         // logic.transpositionTable[positionHash] = bestScore;

//         return bestScore;
//     }

int BestEvalMoveStrategy::betaAlphaMinimax(ChessLogic &logic, int beta, int alpha, EvaluationStrategy* evalStrategy, bool isWhite, 
    short depth, std::chrono::time_point<std::chrono::steady_clock> stopTime) {

        std::vector<ChessLogic::Move> legalMoves = logic.getLegalMoves(isWhite);
        
        if (legalMoves.size() == 0) {
            if (logic.isInCheck(isWhite)) {
                DEBUG_PRINT("checkmate moves: " << logic.printMoves(logic.getMoveHistory()));
                return isWhite ? -30000 + (depth * -1000) : 30000 + (depth * 1000);
                // return isWhite ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max(); // checkmate
            }
            return 0; // stalemate
        }

        if (depth == 0) {
            
            return evalStrategy->evaluate(&logic, isWhite);
        }

        // uint64_t positionHash = logic.hashPosition(isWhite);

        // if (logic.transpositionTable.find(positionHash) != logic.transpositionTable.end()) {
        //         // DEBUG_PRINT("position already reached...skiping");
        //         return logic.transpositionTable[positionHash]; // Return the cached score
        // }

        std::vector<int> scores;
        int bestScore = isWhite ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
        for (const auto &move : legalMoves) {

            logic.makeMove(move);
            
            // std::vector<ChessLogic::Move> oppMoves = logic.getLegalMoves(!isWhite);
            // if (oppMoves.size() == 0) {
            //     DEBUG_PRINT("opponent has no moves");
            //     if (logic.isInCheck(!isWhite)) {
            //         DEBUG_PRINT("Checkmate found: " << logic.translateMoveToString(move));
            //         logic.undoMove();
            //         return isWhite ? 30000 : -30000; 
            //     }
            // }

            
            int score = betaAlphaMinimax(logic, alpha, beta, evalStrategy, !isWhite, depth - 1, stopTime);
            scores.push_back(score);
            if (isWhite) {  // pick negative score for black & positive for white
                bestScore = std::max(bestScore, score);
                alpha = std::max(alpha, score);
                if (beta <= alpha) {
                    logic.undoMove();
                    break;
                }
            } else {
                bestScore = std::min(bestScore, score);
                beta = std::min(beta, score);
                if (alpha >= beta) {
                    logic.undoMove();
                    break;
                }
            }

            logic.undoMove();
        }
        // logic.transpositionTable[positionHash] = bestScore;
        DEBUG_PRINT("score size: " << scores.size());
        DEBUG_PRINT(printScores(scores, bestScore, isWhite, depth));

        return bestScore;
    }

std::string BestEvalMoveStrategy::printScores(std::vector<int> scores, int bestScore, bool isWhite, short depth) {

    std::string scoreString = "[ ";
    DEBUG_PRINT("score size: " << scores.size());
    for (int i = 0; i < scores.size(); i++) {
        scoreString += std::to_string(scores.at(i));
        scoreString += ", ";
    }
    scoreString += "] best: ";
    scoreString += std::to_string(bestScore);
    scoreString +=  " color: ";
    scoreString += isWhite ? "white" : "black";
    scoreString += " depth: ";
    scoreString += std::to_string(depth);
    DEBUG_PRINT(scoreString);
    return scoreString;
}