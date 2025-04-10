#include "best_eval_move.h"


ChessLogic::evalMove BestEvalMoveStrategy::getBestMove(ChessLogic &logic, EvaluationStrategy* evalStrategy,
                                                   bool isWhite, short searchDepth, 
                                                   std::chrono::time_point<std::chrono::steady_clock> stopTime) {

    std::random_device rd;
    std::mt19937 rGen(rd());   // Mersenne Twister engine

    std::vector<ChessLogic::Move> legalMoves = logic.getLegalMoves(isWhite);
    
    if (legalMoves.empty()) {
        return ChessLogic::evalMove(0, ChessLogic::Move()); // Return a null move if no legal moves are available
    } else if (legalMoves.size() == 1) {
        return ChessLogic::evalMove(0, legalMoves.at(0)); // early return if only one legal move
    }

    std::vector<ChessLogic::Move> bestMoves;
    bestMoves.push_back(ChessLogic::Move()); // null move
    const int low = std::numeric_limits<int>::min();
    const int high = std::numeric_limits<int>::max();
    int bestScore = isWhite ? low : high;
    const int jiggle = 30; // randomize choice between equivalent moves

    for (const auto &move : legalMoves) {
        logic.makeMove(move);
        
        // Perform recursive search
        int score = betaAlphaMinimax(&logic, high, low, evalStrategy, !isWhite, searchDepth - 1, stopTime); // score will be positive for white, negative for black

        logic.undoMove();

        // best score == 100 (white)
        // new score = 80
        // 80 + 25 > 100 and 80 - 25 < 100
        // best score == -220
        // new score == -240
        // -240 + 25 > -220  and < -240 - 25 < -220
        if (score + jiggle >= bestScore && score - jiggle <= bestScore) {
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

ChessLogic::evalMove BestEvalMoveStrategy::getBestMove(ChessLogic &logic, EvaluationStrategy* evalStrategy,
    bool isWhite, short searchDepth, short threadCount, std::chrono::time_point<std::chrono::steady_clock> stopTime) {

    std::random_device rd;
    std::mt19937 rGen(rd());   // Mersenne Twister engine
    std::mutex mtx;
    std::vector<ChessLogic::Move> bestMoves;
    bestMoves.push_back(ChessLogic::Move());
    int bestScore = 0;

    std::vector<ChessLogic::Move> legalMoves = logic.getLegalMoves(isWhite);

    if (legalMoves.empty()) {
        return ChessLogic::evalMove(0, ChessLogic::Move()); // Return a null move if no legal moves are available
    }

    // group searchable moves among the threads
    std::vector<ChessLogic::Move> searchMoves[threadCount];
    for (int i = 0; i < legalMoves.size(); i++) {
        searchMoves[i % threadCount].push_back(ChessLogic::Move(legalMoves.at(i)));
    }

    std::vector<std::thread> threadVec;
    for (int i = 0; i < threadCount; i++) {
        threadVec.push_back(std::thread (&BestEvalMoveStrategy::threadedSearch, this, std::ref(logic), searchMoves[i],
        std::ref(bestMoves), std::ref(bestScore), std::ref(mtx), evalStrategy, isWhite, searchDepth, stopTime));
    }
    
    for (auto &t : threadVec) {
        t.join();
    }

    if (bestMoves.size() > 1) {
        std::uniform_int_distribution<> dis(0, bestMoves.size() - 1); // Uniform distribution in the range [0, size-1]
        // Get random index
        return ChessLogic::evalMove(bestScore, bestMoves.at(dis(rGen)));
    } else {
        return ChessLogic::evalMove(bestScore, bestMoves.back());
    }
}

void BestEvalMoveStrategy::getBestMoveThreaded(ChessLogic &logicBoard, EvaluationStrategy* evalStrategy,
    bool isWhite, std::stack<short> &depthstack, std::vector<ChessLogic::evalMove> &bestMove, std::mutex &mtx, 
    short &lastDepth, std::chrono::time_point<std::chrono::steady_clock> stopTime) {

    std::thread::id this_id = std::this_thread::get_id();

    std::random_device rd;
    std::mt19937 rGen(rd());   // Mersenne Twister engine
    
    ChessLogic logic = ChessLogic(logicBoard.internalBoard, logicBoard.moveStack, logicBoard.castleStack, logicBoard.whiteKCastle,
        logicBoard.whiteQCastle, logicBoard.blackKCastle, logicBoard.blackQCastle, logicBoard.enPassantSquare); // create own copy of the board
    
    ChessLogic::evalMove thinkingMove = ChessLogic::evalMove(0, ChessLogic::Move());
    std::vector<ChessLogic::Move> legalMoves = logic.getLegalMoves(isWhite);

    if (legalMoves.empty()) {
        // update the bestMove
        bestMove.push_back(thinkingMove);
    } else if (legalMoves.size() == 1) {
        bestMove.push_back(ChessLogic::evalMove(0, legalMoves.at(0)));
    }

    while (!depthstack.empty()) {
        short searchDepth = -1;
        mtx.lock();
        if (!depthstack.empty()) {
            searchDepth = depthstack.top();
            depthstack.pop();
        }
        mtx.unlock();
        if (searchDepth == -1) {
            break;
        }

        ChessLogic::evalMove potentialMove = thinkingMove;
        std::vector<ChessLogic::Move> bestMoves;
        bestMoves.push_back(ChessLogic::Move()); // null move
        const int low = std::numeric_limits<int>::min();
        const int high = std::numeric_limits<int>::max();
        int bestScore = isWhite ? low : high;
        const int jiggle = 30; // randomize choice between equivalent moves


        for (const auto &move : legalMoves) {
            logic.makeMove(move);
            
            // Perform recursive search
            int score = betaAlphaMinimax(&logic, high, low, evalStrategy, !isWhite, searchDepth - 1, stopTime); // score will be positive for white, negative for black
    
            logic.undoMove();
    
            // best score == 100 (white)
            // new score = 80
            // 80 + 25 > 100 and 80 - 25 < 100
            // best score == -220
            // new score == -240
            // -240 + 25 > -220  and < -240 - 25 < -220
            if (score + jiggle >= bestScore && score - jiggle <= bestScore) {
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
                break; // Exit early if the time limit is exceeded
            }
        } // end of for loop

        if (bestMoves.size() > 1) {
            std::uniform_int_distribution<> dis(0, bestMoves.size() - 1); // Uniform distribution in the range [0, size-1]
            // Get random index
            potentialMove = ChessLogic::evalMove(bestScore, bestMoves.at(dis(rGen)));
        } else {
            potentialMove = ChessLogic::evalMove(bestScore, bestMoves.back());
        }

        if (std::chrono::steady_clock::now() >= stopTime) {
            if (lastDepth < searchDepth && bestMove.back().score < std::abs(potentialMove.score)) {
                mtx.lock();
                lastDepth = searchDepth;
                bestMove.push_back(potentialMove);
                mtx.unlock();
            }
            break; // Exit early if the time limit is exceeded
        } else {
            if (lastDepth < searchDepth) {
                mtx.lock();
                lastDepth = searchDepth;
                bestMove.push_back(potentialMove);
                mtx.unlock();
            }
        }

    } // end of while loop
 
}

int BestEvalMoveStrategy::betaAlphaMinimax(ChessLogic * logic, int beta, int alpha, EvaluationStrategy* evalStrategy, bool isWhite, 
    short depth, std::chrono::time_point<std::chrono::steady_clock> stopTime) {
        int bestScore = isWhite ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();


        std::vector<ChessLogic::Move> legalMoves = logic->getLegalMoves(isWhite);
       
        if (legalMoves.size() == 0) {
            if (logic->isInCheck(isWhite)) {
                return isWhite ? -30000 + (depth * -1000) : 30000 + (depth * 1000);
            }
            return 0; // stalemate
        }

        if (depth == 0) {
            return evalStrategy->evaluate(logic, isWhite);
        }

        for (const ChessLogic::Move move : legalMoves) {

            logic->makeMove(move);
            
            int score = betaAlphaMinimax(logic, beta, alpha, evalStrategy, !isWhite, depth - 1, stopTime);

            logic->undoMove();

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
                return 0; // Exit early if the time limit is exceeded
            }
        }
        
        return bestScore;
    }

static void threadedTest(BestEvalMoveStrategy * moveStrategy, ChessLogic &logic, const std::vector<ChessLogic::Move> searchMoves) {
    std::cout << "in threaded test" << std::endl;

    ChessLogic myCopy = ChessLogic(logic.internalBoard, logic.moveStack, logic.castleStack, logic.whiteKCastle,
        logic.whiteQCastle, logic.blackKCastle, logic.blackQCastle, logic.enPassantSquare);
        DEBUG_PRINT("copied board");
    for(int i = 0; i < 10000; i++) {
        if (i % 100 == 0) {
            std::cout << "in a loop" << std::endl;
        }
    }
    DEBUG_PRINT(myCopy.printBoard());

    DEBUG_PRINT(myCopy.printMoves(searchMoves));

    std::cout << "end of threaded test" << std::endl;
}

void BestEvalMoveStrategy::threadedSearch(ChessLogic &logicBoard, const std::vector<ChessLogic::Move> searchMoves, 
    std::vector<ChessLogic::Move> &bestMoves, int &bestScore, std::mutex &mtx,  EvaluationStrategy * evalStrategy, 
    bool isWhite, short searchDepth, std::chrono::time_point<std::chrono::steady_clock> stopTime) {

        ChessLogic logic = ChessLogic(logicBoard.internalBoard, logicBoard.moveStack, logicBoard.castleStack, logicBoard.whiteKCastle,
            logicBoard.whiteQCastle, logicBoard.blackKCastle, logicBoard.blackQCastle, logicBoard.enPassantSquare);

        const int low = std::numeric_limits<int>::min();
        const int high = std::numeric_limits<int>::max();
        const int jiggle = 30; // randomize choice between equivalent moves

        for (auto move : searchMoves) {

            logic.makeMove(move);
        
            // // Perform recursive search
            int score = betaAlphaMinimax(&logic, high, low, evalStrategy, !isWhite, searchDepth - 1, stopTime); // score will be positive for white, negative for black
    
            logic.undoMove();

            if (score + jiggle >= bestScore && score - jiggle <= bestScore) {
                mtx.lock(); // thread lock while modifying shared resources
                bestMoves.push_back(move);
                mtx.unlock(); // thread unlock
    
            } else if (isWhite) {
                if (score > bestScore) {
                    mtx.lock(); // thread lock while modifying shared resources
                    bestScore = score;
                    bestMoves.clear();
                    bestMoves.push_back(move);
                    mtx.unlock(); // thread unlock
                }
                
            } else {
                if (score < bestScore) {
                    mtx.lock(); // thread lock while modifying shared resources
                    bestScore = score;
                    bestMoves.clear();
                    bestMoves.push_back(move);
                    mtx.unlock(); // thread unlock
                }
            }

            if (std::chrono::steady_clock::now() >= stopTime) {
                break; // Exit early if the time limit is exceeded
            }
        }


    }