#include "random_move.h"
#include <random>

ChessLogic::Move RandomMoveStrategy::getBestMove(ChessLogic &logic, EvaluationStrategy* evalStrategy,
                                                 bool isWhite, short searchDepth, 
                                                 std::chrono::time_point<std::chrono::steady_clock> stopTime) {
    std::vector<ChessLogic::Move> legalMoves = logic.getLegalMoves(isWhite);
    if (legalMoves.empty()) {
        return ChessLogic::Move(); // Return a null move if no legal moves are available
    }

    // Generate a random index to select a move
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, legalMoves.size() - 1);
    int randomIndex = dis(gen);

    return legalMoves[randomIndex];
}
