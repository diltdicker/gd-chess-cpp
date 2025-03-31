#include "chess_logic.h"
#include <stdexcept>
#include <iostream>

short ChessLogic::getSqureTopLeft(short square) const {
    return (square % 8 != 0 && square >= 8) ? square - 9 : -1;
}

short ChessLogic::getSqureTopRight(short square) const {
    return (square % 8 != 7 && square >= 8) ? square - 7 : -1;
}

short ChessLogic::getSqureBottomLeft(short square) const {
    return (square % 8 != 0 && square < 56) ? square + 7 : -1;
}

short ChessLogic::getSqureBottomRight(short square) const {
    return (square % 8 != 7 && square < 56) ? square + 9 : -1;
}

short ChessLogic::getSqureLeft(short square) const {
    return (square % 8 != 0) ? square - 1 : -1;
}

short ChessLogic::getSqureRight(short square) const {
    return (square % 8 != 7) ? square + 1 : -1;
}

short ChessLogic::getSqureTop(short square) const {
    return (square >= 8) ? square - 8 : -1;
}

short ChessLogic::getSqureBottom(short square) const {
    return (square < 56) ? square + 8 : -1;
}

short ChessLogic::getSqureXYrelative(short square, short x, short y) const {
    short newX = (square % 8) + x;
    short newY = (square / 8) + y;
    if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
        return newY * 8 + newX;
    }
    return -1;
}

bool ChessLogic::isMovePsuedoLegal(const Move &move) const {
    // Ensure the move is within bounds
    if (move.from < 0 || move.from >= 64 || move.to < 0 || move.to >= 64) {
        return false;
    }

    // Ensure the piece being moved belongs to the correct player
    if (internalBoard[move.from].color != move.color) {
        return false;
    }

    // Ensure the destination square is not occupied by the same color
    if (internalBoard[move.to].color == move.color) {
        return false;
    }

    // Handle pawn moves
    if (move.piece == 1) { // Pawn
        short direction = (move.color == 1) ? -8 : 8; // White moves up, Black moves down
        if (move.to == move.from + direction) { // Single square forward
            if (internalBoard[move.to].type != 0) {
                return false; // Cannot move forward into an occupied square
            }
        } else if (move.to == move.from + 2 * direction) { // Double square forward
            if ((move.color == 1 && move.from / 8 != 6) || (move.color == 2 && move.from / 8 != 1)) {
                return false; // Double move only allowed from starting rank
            }
            if (internalBoard[move.to].type != 0 || internalBoard[move.from + direction].type != 0) {
                return false; // Path must be clear
            }
        } else if (move.to == move.from + direction - 1 || move.to == move.from + direction + 1) { // Diagonal capture
            if (internalBoard[move.to].type == 0 && move.moveType != 3) {
                return false; // Must capture a piece or be an en passant move
            }
        } else {
            return false; // Invalid pawn move
        }
    }

    // Handle knight moves
    if (move.piece == 2) { // Knight
        short dx = abs((move.from % 8) - (move.to % 8));
        short dy = abs((move.from / 8) - (move.to / 8));
        if (!(dx == 2 && dy == 1) && !(dx == 1 && dy == 2)) {
            return false; // Knights move in an "L" shape
        }
    }

    // Handle bishop moves
    if (move.piece == 3) { // Bishop
        short dx = abs((move.from % 8) - (move.to % 8));
        short dy = abs((move.from / 8) - (move.to / 8));
        if (dx != dy) {
            return false; // Bishops move diagonally
        }
        // Ensure path is clear
        short stepX = (move.to % 8 > move.from % 8) ? 1 : -1;
        short stepY = (move.to / 8 > move.from / 8) ? 1 : -1;
        for (short x = move.from % 8 + stepX, y = move.from / 8 + stepY;
             x != move.to % 8 && y != move.to / 8; x += stepX, y += stepY) {
            if (internalBoard[y * 8 + x].type != 0) {
                return false;
            }
        }
    }

    // Handle rook moves
    if (move.piece == 4) { // Rook
        if (move.from % 8 != move.to % 8 && move.from / 8 != move.to / 8) {
            return false; // Rooks move in straight lines
        }
        // Ensure path is clear
        short step = (move.from % 8 == move.to % 8) ? 8 : 1;
        short start = std::min(move.from, move.to) + step;
        short end = std::max(move.from, move.to);
        for (short i = start; i < end; i += step) {
            if (internalBoard[i].type != 0) {
                return false;
            }
        }
    }

    // Handle queen moves
    if (move.piece == 5) { // Queen
        short dx = abs((move.from % 8) - (move.to % 8));
        short dy = abs((move.from / 8) - (move.to / 8));
        if (dx != dy && move.from % 8 != move.to % 8 && move.from / 8 != move.to / 8) {
            return false; // Queens move diagonally or in straight lines
        }
        // Ensure path is clear (reuse bishop and rook logic)
        if (dx == dy) { // Diagonal move
            short stepX = (move.to % 8 > move.from % 8) ? 1 : -1;
            short stepY = (move.to / 8 > move.from / 8) ? 1 : -1;
            for (short x = move.from % 8 + stepX, y = move.from / 8 + stepY;
                 x != move.to % 8 && y != move.to / 8; x += stepX, y += stepY) {
                if (internalBoard[y * 8 + x].type != 0) {
                    return false;
                }
            }
        } else { // Straight line move
            short step = (move.from % 8 == move.to % 8) ? 8 : 1;
            short start = std::min(move.from, move.to) + step;
            short end = std::max(move.from, move.to);
            for (short i = start; i < end; i += step) {
                if (internalBoard[i].type != 0) {
                    return false;
                }
            }
        }
    }

    // Handle king moves
    if (move.piece == 6) { // King
        short dx = abs((move.from % 8) - (move.to % 8));
        short dy = abs((move.from / 8) - (move.to / 8));
        if (dx > 1 || dy > 1) {
            return false; // Kings move one square in any direction
        }
        // Handle castling
        if (move.moveType == 1 || move.moveType == 2) { // Castling
            // Ensure the path is clear and the rook is in the correct position
            short rookSquare = (move.moveType == 1) ? move.from + 3 : move.from - 4;
            short step = (move.moveType == 1) ? 1 : -1;

            // Check castling rights
            if ((move.color == 1 && move.moveType == 1 && !whiteKCastle) || 
                (move.color == 1 && move.moveType == 2 && !whiteQCastle) || 
                (move.color == 2 && move.moveType == 1 && !blackKCastle) || 
                (move.color == 2 && move.moveType == 2 && !blackQCastle)) {
                return false; // Castling not allowed
            }

            for (short i = move.from + step; i != rookSquare; i += step) {
                if (internalBoard[i].type != 0) {
                    return false; // Path must be clear
                }
            }
            if (internalBoard[rookSquare].type != 4 || internalBoard[rookSquare].color != move.color) {
                return false; // Rook must be in the correct position
            }
        }
    }

    return true; // If all checks pass, the move is pseudo-legal
}

u_int64_t ChessLogic::getPieceBitBoard(short color, short piece) const {
    u_int64_t bitboard = 0;
    for (short i = 0; i < 64; ++i) {
        if (internalBoard[i].color == color && internalBoard[i].type == piece) {
            bitboard |= (1ULL << i);
        }
    }
    return bitboard;
}

u_int64_t ChessLogic::getPieceBitBoard(short color) const {
    u_int64_t bitboard = 0;
    for (short i = 0; i < 64; ++i) {
        if (internalBoard[i].color == color) {
            bitboard |= (1ULL << i);
        }
    }
    return bitboard;
}

ChessLogic::ChessLogic() {
    // Initialize the internal board with empty pieces
    for (int i = 0; i < 64; ++i) {
        internalBoard[i] = {0, 0}; // Empty piece
    }
}

ChessLogic::~ChessLogic() {
    // Destructor logic if needed
}

std::vector<ChessLogic::Move> ChessLogic::get_legal_moves(bool isWhite) {
    std::vector<Move> legalMoves;
    short color = isWhite ? 1 : 2;

    for (short i = 0; i < 64; ++i) {
        if (internalBoard[i].color == color) {
            for (short j = 0; j < 64; ++j) {
                Move move = translateMove(i, j);
                if (isMoveLegal(move)) {
                    legalMoves.push_back(move);
                }
            }
        }
    }

    return legalMoves;
}

void ChessLogic::copyChessBoard(const chessPiece inputBoard[64]) {
    for (int i = 0; i < 64; ++i) {
        internalBoard[i] = inputBoard[i];
    }
}

void ChessLogic::make_move(const Move &move) {
    // Push the move onto the stack for undo functionality
    moveStack.push(move);

    // Handle castling
    if (move.moveType == 1) { // Kingside castling
        internalBoard[move.to] = internalBoard[move.from]; // Move king
        internalBoard[move.from] = {0, 0}; // Clear king's original square
        internalBoard[move.to - 1] = internalBoard[move.to + 1]; // Move rook
        internalBoard[move.to + 1] = {0, 0}; // Clear rook's original square
        return;
    } else if (move.moveType == 2) { // Queenside castling
        internalBoard[move.to] = internalBoard[move.from]; // Move king
        internalBoard[move.from] = {0, 0}; // Clear king's original square
        internalBoard[move.to + 1] = internalBoard[move.to - 2]; // Move rook
        internalBoard[move.to - 2] = {0, 0}; // Clear rook's original square
        return;
    }

    // Handle en passant
    if (move.moveType == 3) { // En passant
        internalBoard[move.to] = internalBoard[move.from]; // Move pawn
        internalBoard[move.from] = {0, 0}; // Clear pawn's original square
        short capturedPawnSquare = (move.color == 1) ? move.to + 8 : move.to - 8;
        internalBoard[capturedPawnSquare] = {0, 0}; // Clear the captured pawn
        return;
    }

    // Update the internal board for normal moves
    internalBoard[move.to] = internalBoard[move.from];
    internalBoard[move.to].type = move.promotion ? move.promotion : internalBoard[move.from].type;
    internalBoard[move.from] = {0, 0}; // Clear the starting square

    // Update castling rights
    if (move.from == 60) { // White king moves
        whiteQCastle = false;
        whiteKCastle = false;
    } else if (move.from == 56) { // White queenside rook moves
        whiteQCastle = false;
    } else if (move.from == 63) { // White kingside rook moves
        whiteKCastle = false;
    } else if (move.from == 4) { // Black king moves
        blackQCastle = false;
        blackKCastle = false;
    } else if (move.from == 0) { // Black queenside rook moves
        blackQCastle = false;
    } else if (move.from == 7) { // Black kingside rook moves
        blackKCastle = false;
    }

    // Update en passant square
    if (move.moveType == 4) { // Pawn double move
        enPassantSquare = (move.from + move.to) / 2; // Set en passant square
    } else {
        enPassantSquare = -1; // Reset en passant square
    }
}

bool ChessLogic::isMoveLegal(const Move &move) {
    if (!isMovePsuedoLegal(move)) {
        return false; // Move is not pseudo-legal
    }
    bool isLegal = true;
    // Temporarily make the move
    make_move(move);

    if (isInCheck(move.color)) {
        isLegal = false; // Move leaves the king in check
    }

    undo_move(); // Undo the move

    // Additional checks for castling
    if (isLegal && move.piece == 6 && (move.moveType == 1 || move.moveType == 2)) { // King castling
        short step = (move.moveType == 1) ? 1 : -1; // Kingside or Queenside
        short kingSquare = move.from;

        // Check if the king passes through or lands on a square under attack
        for (short i = 0; i <= 2; ++i) { // Check up to 2 squares (including the destination)
            if (isInCheck(move.color)) {
                isLegal = false; // King passes through or lands on a square under attack
                break;
            }
            kingSquare += step; // Move the king to the next square

            ChessLogic::Move stepMove = Move();
            stepMove.from = move.from;
            stepMove.to = kingSquare;
            stepMove.piece = move.piece;
            stepMove.color = move.color;
            make_move(stepMove); // Temporarily move the king
        }

        // Undo the temporary moves
        for (short i = 0; i <= 2; ++i) {
            undo_move();
        }
    }

    return isLegal;
}

void ChessLogic::undo_move() {
    if (moveStack.empty()) {
        return;
    }

    // Pop the last move from the stack
    Move lastMove = moveStack.top();
    moveStack.pop();

    // Undo the move on the internal board
    internalBoard[lastMove.from] = internalBoard[lastMove.to];
    internalBoard[lastMove.from].type = lastMove.piece; // Restore the original piece type
    internalBoard[lastMove.to] = {0, 0}; // Clear the destination square

    // Handle captures
    if (lastMove.capture) {
        internalBoard[lastMove.to] = {static_cast<short>(lastMove.capture), static_cast<short>(lastMove.color == 1 ? 2 : 1)};
    }

    // Handle castling
    if (lastMove.moveType == 1) { // Kingside castling
        internalBoard[lastMove.to + 1] = internalBoard[lastMove.to - 1]; // Move rook back
        internalBoard[lastMove.to - 1] = {0, 0}; // Clear rook's new square
    } else if (lastMove.moveType == 2) { // Queenside castling
        internalBoard[lastMove.to - 2] = internalBoard[lastMove.to + 1]; // Move rook back
        internalBoard[lastMove.to + 1] = {0, 0}; // Clear rook's new square
    }

    // Handle en passant
    if (lastMove.moveType == 3) { // En passant
        short capturedPawnSquare = (lastMove.color == 1) ? lastMove.to + 8 : lastMove.to - 8;
        internalBoard[capturedPawnSquare] = {static_cast<short>(lastMove.capture), static_cast<short>(lastMove.color == 1 ? 2 : 1)};
        internalBoard[lastMove.to] = {0, 0}; // Clear the en passant capture square
    }

    // Handle pawn promotion
    if (lastMove.piece == 1 && lastMove.promotion != 0) { // If the move was a pawn promotion
        internalBoard[lastMove.from].type = 1; // Restore the piece type to pawn
    }

    // Restore castling rights
    if (lastMove.from == 60) { // White king moves
        whiteQCastle = true;
        whiteKCastle = true;
    } else if (lastMove.from == 56) { // White queenside rook moves
        whiteQCastle = true;
    } else if (lastMove.from == 63) { // White kingside rook moves
        whiteKCastle = true;
    } else if (lastMove.from == 4) { // Black king moves
        blackQCastle = true;
        blackKCastle = true;
    } else if (lastMove.from == 0) { // Black queenside rook moves
        blackQCastle = true;
    } else if (lastMove.from == 7) { // Black kingside rook moves
        blackKCastle = true;
    }

    // Restore en passant square
    if (!moveStack.empty()) {
        const Move &previousMove = moveStack.top();
        enPassantSquare = (previousMove.moveType == 4) ? (previousMove.from + previousMove.to) / 2 : -1;
    } else {
        enPassantSquare = -1; // Reset if no previous move
    }
}

void ChessLogic::emtpyMoveStack() {
    while (!moveStack.empty()) {
        moveStack.pop();
    }
}

ChessLogic::Move ChessLogic::translateMove(short fromSquare, short toSquare) const {
    Move move;
    move.from = fromSquare;
    move.to = toSquare;
    move.piece = internalBoard[fromSquare].type;
    move.color = internalBoard[fromSquare].color;

    // Check for captures
    if (internalBoard[toSquare].type != 0) {
        move.capture = internalBoard[toSquare].type;
    } else {
        move.capture = 0;
    }

    // Determine move type (e.g., castling, en passant, pawn double move)
    move.moveType = 0; // Default to normal move
    if (move.piece == 6 && abs(fromSquare - toSquare) == 2) { // King castling
        move.moveType = (toSquare > fromSquare) ? 1 : 2; // Kingside or Queenside
    } else if (move.piece == 1 && abs(fromSquare - toSquare) == 16) { // Pawn double move
        move.moveType = 4;
    } else if (move.piece == 1 && internalBoard[toSquare].type == 0 && abs(fromSquare - toSquare) % 8 != 0) { // En passant
        move.moveType = 3;
        move.capture = 1; // Pawn captured
    }

    return move;
}

ChessLogic::Move ChessLogic::translateMove(const std::string &moveStr) const {
    if (moveStr.length() < 4) {
        std::cerr << "Error: Invalid move string provided to translateMove." << std::endl;
        std::abort();
    }

    short fromSquare = (moveStr[1] - '1') * 8 + (moveStr[0] - 'a');
    short toSquare = (moveStr[3] - '1') * 8 + (moveStr[2] - 'a');

    Move move = translateMove(fromSquare, toSquare);

    // Handle promotion
    if (moveStr.length() == 5) {
        switch (moveStr[4]) {
            case 'n': move.promotion = 2; break; // Knight
            case 'b': move.promotion = 3; break; // Bishop
            case 'r': move.promotion = 4; break; // Rook
            case 'q': move.promotion = 5; break; // Queen
            std::cerr << "Invalid promotion piece" << std::endl;
            std::abort();
        }
    } else {
        move.promotion = 0; // No promotion
    }

    return move;
}

std::string ChessLogic::translateMoveToString(const Move &move) const {
    // Check for a null move
    if (move.from == -1 && move.to == -1) {
        return "0000"; // Null move representation
    }

    std::string moveStr;

    // Convert the 'from' square to algebraic notation
    moveStr += static_cast<char>('a' + (move.from % 8)); // File
    moveStr += static_cast<char>('1' + (move.from / 8)); // Rank

    // Convert the 'to' square to algebraic notation
    moveStr += static_cast<char>('a' + (move.to % 8)); // File
    moveStr += static_cast<char>('1' + (move.to / 8)); // Rank

    // Add promotion piece if applicable
    if (move.promotion != 0) {
        switch (move.promotion) {
            case 2: moveStr += 'n'; break; // Knight
            case 3: moveStr += 'b'; break; // Bishop
            case 4: moveStr += 'r'; break; // Rook
            case 5: moveStr += 'q'; break; // Queen
        }
    }

    return moveStr;
}

const ChessLogic::chessPiece* ChessLogic::getChessBoard() const {
    return internalBoard;
}

bool ChessLogic::isInCheck(short color) const {
    // Find the king's position
    short kingSquare = -1;
    for (short i = 0; i < 64; ++i) {
        if (internalBoard[i].type == 6 && internalBoard[i].color == color) {
            kingSquare = i;
            break;
        }
    }

    if (kingSquare == -1) {
        return false; // No king found, technically not in check
    }

    short opponentColor = (color == 1) ? 2 : 1;

    // Check for attacks from pawns
    short pawnDirection = (color == 1) ? -8 : 8;
    if (getSqureXYrelative(kingSquare, -1, pawnDirection / 8) != -1 &&
        internalBoard[getSqureXYrelative(kingSquare, -1, pawnDirection / 8)].type == 1 &&
        internalBoard[getSqureXYrelative(kingSquare, -1, pawnDirection / 8)].color == opponentColor) {
        return true;
    }
    if (getSqureXYrelative(kingSquare, 1, pawnDirection / 8) != -1 &&
        internalBoard[getSqureXYrelative(kingSquare, 1, pawnDirection / 8)].type == 1 &&
        internalBoard[getSqureXYrelative(kingSquare, 1, pawnDirection / 8)].color == opponentColor) {
        return true;
    }

    // Check for attacks from knights
    const short knightMoves[8][2] = {{-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}};
    for (const auto &move : knightMoves) {
        short targetSquare = getSqureXYrelative(kingSquare, move[0], move[1]);
        if (targetSquare != -1 && internalBoard[targetSquare].type == 2 && internalBoard[targetSquare].color == opponentColor) {
            return true;
        }
    }

    // Check for attacks from bishops and queens (diagonal)
    const short bishopDirections[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    for (const auto &dir : bishopDirections) {
        short targetSquare = kingSquare;
        while ((targetSquare = getSqureXYrelative(targetSquare, dir[0], dir[1])) != -1) {
            if (internalBoard[targetSquare].type != 0) {
                if ((internalBoard[targetSquare].type == 3 || internalBoard[targetSquare].type == 5) &&
                    internalBoard[targetSquare].color == opponentColor) {
                    return true;
                }
                break;
            }
        }
    }

    // Check for attacks from rooks and queens (straight lines)
    const short rookDirections[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (const auto &dir : rookDirections) {
        short targetSquare = kingSquare;
        while ((targetSquare = getSqureXYrelative(targetSquare, dir[0], dir[1])) != -1) {
            if (internalBoard[targetSquare].type != 0) {
                if ((internalBoard[targetSquare].type == 4 || internalBoard[targetSquare].type == 5) &&
                    internalBoard[targetSquare].color == opponentColor) {
                    return true;
                }
                break;
            }
        }
    }

    // Check for attacks from the opponent king
    const short kingMoves[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    for (const auto &move : kingMoves) {
        short targetSquare = getSqureXYrelative(kingSquare, move[0], move[1]);
        if (targetSquare != -1 && internalBoard[targetSquare].type == 6 && internalBoard[targetSquare].color == opponentColor) {
            return true;
        }
    }

    return false; // No attacks found, king is not in check
}
