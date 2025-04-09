#include "best_eval_move.h"


ChessLogic::evalMove BestEvalMoveStrategy::getBestMove(ChessLogic &logic, EvaluationStrategy* evalStrategy,
                                                   bool isWhite, short searchDepth, 
                                                   std::chrono::time_point<std::chrono::steady_clock> stopTime) {

    std::random_device rd;
    std::mt19937 rGen(rd());   // Mersenne Twister engine

    std::vector<ChessLogic::Move> legalMoves = logic.getLegalMoves(isWhite);
    
    if (legalMoves.empty()) {
        return ChessLogic::evalMove(0, ChessLogic::Move()); // Return a null move if no legal moves are available
    }

    std::vector<ChessLogic::Move> bestMoves;
    bestMoves.push_back(ChessLogic::Move()); // null move
    int low = std::numeric_limits<int>::min();
    int high = std::numeric_limits<int>::max();
    int bestScore = isWhite ? low : high;
    

    for (const auto &move : legalMoves) {
        logic.makeMove(move);
        
        // Perform recursive search
        int score = betaAlphaMinimax(logic, high, low, evalStrategy, !isWhite, searchDepth - 1, stopTime); // score will be positive for white, negative for black

        logic.undoMove();

        if (score == bestScore) {
            bestMoves.push_back(move);

        } else if (isWhite) {
            if (score > bestScore) {
                bestScore = score;
                bestMoves.clear();
                bestMoves.push_back(move);
            }
            
        } else {
            if (score < bestScore) {
                bestScore = score;
                bestMoves.clear();
                bestMoves.push_back(move);
            }
        }

        if (std::chrono::steady_clock::now() >= stopTime) {
            DEBUG_PRINT("Time limit exit: getBestMove");
            break; // Exit early if the time limit is exceeded
        }
    }

    if (bestMoves.size() > 1) {
        std::uniform_int_distribution<> dis(0, bestMoves.size() - 1); // Uniform distribution in the range [0, size-1]
        // Get random index
        return ChessLogic::evalMove(bestScore, bestMoves.at(dis(rGen)));
    } else {
        return ChessLogic::evalMove(bestScore, bestMoves.back());
    }
    
}


int BestEvalMoveStrategy::betaAlphaMinimax(ChessLogic &logic, int beta, int alpha, EvaluationStrategy* evalStrategy, bool isWhite, 
    short depth, std::chrono::time_point<std::chrono::steady_clock> stopTime) {
        int bestScore = isWhite ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();


        std::vector<ChessLogic::Move> legalMoves = logic.getLegalMoves(isWhite);
       
        if (legalMoves.size() == 0) {
            if (logic.isInCheck(isWhite)) {
                return isWhite ? -30000 + (depth * -1000) : 30000 + (depth * 1000);
            }
            return 0; // stalemate
        }

        // DEBUG_PRINT("getBestMove presort moves: " << logic.printMoves(legalMoves));
        
        // DEBUG_PRINT("getBestMove aftsort moves: " << logic.printMoves(legalMoves));

        if (depth == 0) {
            return evalStrategy->evaluate(&logic, isWhite);
        }

        // if (legalMoves.size() > 1) {
        //     std::sort(legalMoves.begin(), legalMoves.end(), [](ChessLogic::Move a, ChessLogic::Move b) {
        //         int ptsA = 7 - a.piece * (1 + a.capture);
        //         int ptsB = 7 - b.piece * (1 + b.capture);
        //         return ptsA >= ptsB;
        //     });
        // }

        for (const ChessLogic::Move move : legalMoves) {

            logic.makeMove(move);
            
            int score = betaAlphaMinimax(logic, beta, alpha, evalStrategy, !isWhite, depth - 1, stopTime);

            logic.undoMove();

            if (isWhite) {  // pick negative score for black & positive for white

                bestScore = std::max(bestScore, score);
                alpha = std::max(alpha, score);
                if (beta <= alpha) {
                    break;
                }

            } else {
                bestScore = std::min(bestScore, score);
                beta = std::min(beta, score);
                if (alpha >= beta) {
                    break;
                }
            }

            
        }
        
        return bestScore;
    }