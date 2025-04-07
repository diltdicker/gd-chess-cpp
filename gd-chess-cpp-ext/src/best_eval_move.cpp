#include "best_eval_move.h"


ChessLogic::Move BestEvalMoveStrategy::getBestMove(ChessLogic &logic, EvaluationStrategy* evalStrategy,
                                                   bool isWhite, short searchDepth, 
                                                   std::chrono::time_point<std::chrono::steady_clock> stopTime) {
    std::vector<ChessLogic::Move> legalMoves = logic.getLegalMoves(isWhite);
    if (legalMoves.empty()) {
        return ChessLogic::Move(); // Return a null move if no legal moves are available
    }
    DEBUG_PRINT("allowed moves: " << logic.printMoves(legalMoves));

    std::vector<ChessLogic::Move> bestMoves;
    bestMoves.push_back(ChessLogic::Move()); // null move
    int low = std::numeric_limits<int>::min();
    int high = std::numeric_limits<int>::max();
    int bestScore = isWhite ? low : high;
    

    for (const auto &move : legalMoves) {
        logic.makeMove(move);

        // if (logic.isInCheck(!isWhite) && logic.getLegalMoves(!isWhite).size() == 0) { // Skip moves that put the king in check
        //     DEBUG_PRINT("Fist move checkmate: " << logic.translateMoveToString(move));
        //     logic.undoMove();
        //     return move; // Return the move that puts the opponent in checkmate
        // }
        
        // Perform recursive search
        int score = betaAlphaMinimax(logic, high, low, evalStrategy, !isWhite, searchDepth - 1, stopTime); // score will be positive for white, negative for black

        DEBUG_PRINT("evaluating move: " << logic.translateMoveToString(move) << "score: " << score);

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

    DEBUG_PRINT("final moves to choose from: " << logic.printMoves(bestMoves));
    // ChessLogic::Move
    std::random_device rd;  // Seed from hardware
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine
    std::uniform_int_distribution<> dis(0, bestMoves.size() - 1); // Uniform distribution in the range [0, size-1]

    // Get random index
    return bestMoves.at(dis(gen));
}


int BestEvalMoveStrategy::betaAlphaMinimax(ChessLogic &logic, int beta, int alpha, EvaluationStrategy* evalStrategy, bool isWhite, 
    short depth, std::chrono::time_point<std::chrono::steady_clock> stopTime) {
        int bestScore = isWhite ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

        

        // uint64_t positionHash = logic.hashPosition(isWhite);

        // if (logic.transpositionTable.find(positionHash) != logic.transpositionTable.end()) {
        //     return logic.transpositionTable[positionHash]; // Return the cached score
        // }

        const std::vector<ChessLogic::Move> legalMoves = logic.getLegalMoves(isWhite);
        if (legalMoves.size() == 0) {
            if (logic.isInCheck(isWhite)) {
                DEBUG_PRINT("found checkmate " << logic.printMoves(logic.getMoveHistory()));
                return isWhite ? -30000 + (depth * -1000) : 30000 + (depth * 1000);
            }
            return 0; // stalemate
        }

        if (depth == 0) {
            return evalStrategy->evaluate(&logic, isWhite);
        }

        

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

            if (std::chrono::steady_clock::now() >= stopTime) {
                DEBUG_PRINT("Time limit exit: getBestMove");
                break; // Exit early if the time limit is exceeded
            }

            
        }
        
        // Store result in transposition table
        // logic.transpositionTable[positionHash] = bestScore;
        return bestScore;
    }