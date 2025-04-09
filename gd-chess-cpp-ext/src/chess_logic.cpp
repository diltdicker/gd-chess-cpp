#include "chess_logic.h"

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
        short relativeSqure = (newY * 8) + newX;
        if (relativeSqure >= 0 && relativeSqure < 64) {
            return relativeSqure;
        }
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
        // DEBUG_PRINT("pawn move: " << translateMoveToString(move) << " to suare: " << move.to << " from square: " << move.from);
        short direction = (move.color == 1) ? -8 : 8; // White moves up, Black moves down
        if (move.to == move.from + direction) { // Single square forward
            if (internalBoard[move.to].type != 0) {
                return false; // Cannot move forward into an occupied square
            }
        } else if (move.to == move.from + 2 * direction) { // Double square forward
            // DEBUG_PRINT("pawn doublevc move: " << translateMoveToString(move));
            if ((move.color == 1 && move.from / 8 != 6) || (move.color == 2 && move.from / 8 != 1)) {
                return false; // Double move only allowed from starting rank
            }
            if (internalBoard[move.to].type != 0 || internalBoard[move.from + direction].type != 0) {
                return false; // Path must be clear
            }
        } else if ((move.to == move.from + direction - 1 && move.from % 8 != 0) || (move.to == move.from + direction + 1 && move.from % 8 != 7)) { // Diagonal capture
            // DEBUG_PRINT("pawn attack move: " << translateMoveToString(move));
            if (internalBoard[move.to].type == 0) {
                if (move.to != enPassantSquare || move.moveType != 3) {
                    return false; // Must capture a piece or be a valid en passant move
                }
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

uint64_t ChessLogic::getPieceBitBoard(short color, short piece) const {
    uint64_t bitboard = 0;
    for (short i = 0; i < 64; ++i) {
        if (internalBoard[i].color == color && internalBoard[i].type == piece) {
            bitboard |= (1ULL << i);
        }
    }
    return bitboard;
}

uint64_t ChessLogic::getColorBitBoard(short color) const {
    uint64_t bitboard = 0;
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

void ChessLogic::copyChessBoard(const chessPiece inputBoard[64]) {
    for (int i = 0; i < 64; ++i) {
        internalBoard[i] = inputBoard[i];
    }
}

std::vector<ChessLogic::Move> ChessLogic::getLegalMoves(bool isWhite) {
    std::vector<Move> pseudoMoves;
    short color = isWhite ? 1 : 2;
    short opponentColor = isWhite ? 2 : 1;

    uint64_t colorBitboard = getColorBitBoard(color);
    std::vector<short> fromSquares = bitboardToSquares(colorBitboard); // own squares
    std::vector<short> toSquares = bitboardToSquares(
        getPawnMoveBitBoard(color) | getKnightMoveBitBoard(color) | getBishopMoveBitBoard(color) |
        getRookMoveBitBoard(color) | getKingMoveBitBoard(color)
    ); 
    
    for (short i = 0; i < fromSquares.size() ; ++i) {
        
        if (internalBoard[fromSquares.at(i)].color == color) {
            for (short j = 0; j < toSquares.size(); ++j) {

                Move move = translateMove(fromSquares.at(i), toSquares.at(j));

                if (isMovePsuedoLegal(move)) {

                    if (move.piece == 1) { // add promotions
                        if ((move.color == 1 && move.to / 8 == 7) || (move.color == 2 && move.to / 8 == 0)) {
                            Move queenPromo = Move(move);
                            queenPromo.promotion = 5;
                            Move rookPromo = Move(move);
                            rookPromo.promotion = 4;
                            Move bishopPromo = Move(move);
                            bishopPromo.promotion = 3;
                            Move kightPromo = Move(move);
                            kightPromo.promotion = 2;
                            pseudoMoves.push_back(queenPromo);
                            pseudoMoves.push_back(rookPromo);
                            pseudoMoves.push_back(bishopPromo);
                            pseudoMoves.push_back(kightPromo);
                        } else {

                            pseudoMoves.push_back(move); // normal pawn move
                        }

                    } else {
                        pseudoMoves.push_back(move);
                    }
                    
                }
            }
        }
    }
    // sort moves based on captures and pieces
    if (pseudoMoves.size() > 8) {
        std::stable_sort(pseudoMoves.begin(), pseudoMoves.end(), [](ChessLogic::Move a, ChessLogic::Move b) {
            if (a.capture || b.capture) {
                return a.capture > b.capture;
            }
            if (a.piece == 1 && b.piece == 1) {
               return (a.to % 8 > 2 && a.to % 8 < 5); // flank pawns last 
            }
            return (a.piece < b.piece && a.piece != 1);
        });
    }

    std::vector<Move> legalMoves;

    for (const auto move : pseudoMoves) {
        if (isMoveLegal(move)) {
            legalMoves.push_back(move);
        }
    }

    return legalMoves;
}

void ChessLogic::makeMove(const Move &move) {

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
    internalBoard[move.to] = internalBoard[move.from]; // Move the piece
    if (move.promotion != 0) {
        internalBoard[move.to].type = move.promotion; // Promote the pawn
    }
    internalBoard[move.from] = {0, 0}; // Clear the starting square

    castleStack.push(castleRights(whiteKCastle, whiteQCastle, blackKCastle, blackQCastle)); // record the previous castling rights
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
    bool isLegal = true;
    // Temporarily make the move
    makeMove(move);

    if (isInCheck(move.color == 1)) {
        isLegal = false; // Move leaves the king in check
    }

    undoMove(); // Undo the move

    // Additional checks for castling
    if (isLegal && move.piece == 6 && (move.moveType == 1 || move.moveType == 2)) { // King castling
        short step = (move.moveType == 1) ? 1 : -1; // Kingside or Queenside
        short kingSquare = move.from;

        // Check if the king passes through or lands on a square under attack
        for (short i = 0; i <= 2; ++i) { // Check up to 2 squares (including the destination)
            if (isInCheck(move.color == 1)) {
                isLegal = false; // King passes through or lands on a square under attack
                break;
            }
            kingSquare += step; // Move the king to the next square

            ChessLogic::Move stepMove = Move();
            stepMove.from = move.from;
            stepMove.to = kingSquare;
            stepMove.piece = move.piece;
            stepMove.color = move.color;
            makeMove(stepMove); // Temporarily move the king
        }

        // Undo the temporary moves
        for (short i = 0; i <= 2; ++i) {
            undoMove();
        }
    }

    return isLegal;
}

void ChessLogic::undoMove() {
    if (moveStack.empty()) {
        return;
    }

    // Pop the last move from the stack
    Move lastMove = moveStack.top();
    moveStack.pop();

    // Undo the move on the internal board
    internalBoard[lastMove.from] = chessPiece(lastMove.color, lastMove.piece);
    internalBoard[lastMove.to] = {0, 0}; // Clear the destination square

    // Handle captures
    if (lastMove.capture) {
        internalBoard[lastMove.to] = chessPiece(lastMove.color == 1 ? 2 : 1, lastMove.capture);
    }

    // Handle castling
    if (lastMove.moveType == 1) { // Kingside castling
        internalBoard[lastMove.to + 1] = chessPiece(internalBoard[lastMove.to - 1].color, 4); // Move rook back
        internalBoard[lastMove.to - 1] = {0, 0}; // Clear rook's new square
    } else if (lastMove.moveType == 2) { // Queenside castling
        internalBoard[lastMove.to - 2] = chessPiece(internalBoard[lastMove.to + 1].color, 4); // Move rook back
        internalBoard[lastMove.to + 1] = {0, 0}; // Clear rook's new square
    }

    // Handle en passant
    if (lastMove.moveType == 3) { // En passant
        short capturedPawnSquare = (lastMove.color == 1) ? lastMove.to + 8 : lastMove.to - 8;
        internalBoard[capturedPawnSquare] = chessPiece(lastMove.color == 1 ? 2 :1, 1);
        internalBoard[lastMove.to] = {0, 0}; // Clear the en passant capture square
    }

    // Handle pawn promotion
    if (lastMove.piece == 1 && lastMove.promotion != 0) { // If the move was a pawn promotion
        internalBoard[lastMove.from].type = 1; // Restore the piece type to pawn
    }

    // Restore castling rights
    if (!castleStack.empty()) {
        castleRights oldRights = castleStack.top();
        castleStack.pop();
        whiteKCastle = oldRights.wKingside;
        whiteQCastle = oldRights.wQueenside;
        blackKCastle = oldRights.bKingside;
        blackQCastle = oldRights.bQueenside;
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
    while (!castleStack.empty())
    {
        castleStack.pop();
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
    return Move(move);
}

ChessLogic::Move ChessLogic::translateMove(const std::string &moveStr) const {
    if (moveStr.length() < 4) {
        std::cerr << "Error: Invalid move string provided to translateMove." << std::endl;
        std::abort();
    }

    short fromSquare = stringToSquare(moveStr.substr(0,2));
    short toSquare = stringToSquare(moveStr.substr(2,2));

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

    moveStr += squareToString(move.from); // Convert the 'from' square to algebraic notation
    moveStr += squareToString(move.to);   // Convert the 'to' square to algebraic notation

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

bool ChessLogic::isInCheck(bool isWhite) const {

    short color = isWhite ? 1 : 2;

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
        targetSquare = getSqureXYrelative(targetSquare, dir[0], dir[1]);
        while (targetSquare != -1) {
            if (internalBoard[targetSquare].type != 0) {
                if ((internalBoard[targetSquare].type == 4 || internalBoard[targetSquare].type == 5) &&
                    internalBoard[targetSquare].color == opponentColor) {
                    return true;
                }
                break;
            }
            targetSquare = getSqureXYrelative(targetSquare, dir[0], dir[1]);
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

std::vector<short> ChessLogic::bitboardToSquares(uint64_t bitboard) const {
    std::vector<short> squares;
    for (short i = 0; i < 64; ++i) {
        if (bitboard & (1ULL << i)) {
            squares.push_back(i);
        }
    }
    return squares;
}

uint64_t ChessLogic::getPawnMoveBitBoard(short color) const {
    uint64_t pawnBitboard = getPieceBitBoard(color, 1); // Get bitboard for pawns
    uint64_t emptySquares = ~getColorBitBoard(1) & ~getColorBitBoard(2); // Empty squares
    uint64_t opponentBitboard = getColorBitBoard(color == 1 ? 2 : 1); // Opponent pieces

    uint64_t singlePush = 0;
    uint64_t doublePush = 0;
    uint64_t capturesLeft = 0;
    uint64_t capturesRight = 0;

    if (color == 1) { // White pawns
        singlePush = (pawnBitboard >> 8) & emptySquares; // Move one square forward
        doublePush = ((singlePush & 0x0000FF0000000000) >> 8) & emptySquares; // Move two squares forward from rank 2
        capturesLeft = (pawnBitboard >> 9) & ~0x0101010101010101; // Diagonal left capture
        capturesRight = (pawnBitboard >> 7) & ~0x8080808080808080; // Diagonal right capture
        
    } else { // Black pawns
        singlePush = (pawnBitboard << 8) & emptySquares; // Move one square forward
        doublePush = ((singlePush & 0x0000000000FF0000) << 8) & emptySquares; // Move two squares forward from rank 7
        capturesLeft = (pawnBitboard << 7) & ~0x0101010101010101; // Diagonal left capture
        capturesRight = (pawnBitboard << 9) & ~0x8080808080808080; // Diagonal right capture
    }

    // Combine all possible moves
    return singlePush | doublePush | capturesLeft | capturesRight;
}

uint64_t ChessLogic::getKnightMoveBitBoard(short color) const {
    uint64_t knightBitboard = getPieceBitBoard(color, 2); // Get bitboard for knights
    uint64_t emptySquares = ~getColorBitBoard(1) & ~getColorBitBoard(2); // Empty squares
    uint64_t opponentBitboard = getColorBitBoard(color == 1 ? 2 : 1); // Opponent pieces

    uint64_t knightMoves = 0;
    const short knightOffsets[8] = {-17, -15, -10, -6, 6, 10, 15, 17};

    for (short i = 63; i >= 0; i--) {

        if (knightBitboard & (1ULL << i)) {

            for (short j = 0; j < 8; ++j) {
                short targetSquare = i + knightOffsets[j];
                if (targetSquare >= 0 && targetSquare < 64) {
                    if ((knightOffsets[j] == -10 && (i % 8 <= 1)) // queenside check
                        || (knightOffsets[j] == 6 && (i % 8 <= 1))
                        || (knightOffsets[j] == -17 && (i % 8 == 0))
                        || (knightOffsets[j] == 15 && (i % 8 == 0))
                        || (knightOffsets[j] == -6 && (i % 8 >= 6)) // kingside check
                        || (knightOffsets[j] == 10 && (i % 8 >= 6)) 
                        || (knightOffsets[j] == 17 && (i % 8 == 7)) 
                        || (knightOffsets[j] == -15 && (i % 8 == 7))) {
                            continue; // Skip invalid moves
                    }
                    knightMoves |= (1ULL << targetSquare);
                }
            }
            
        }
    }

    return knightMoves | opponentBitboard;
}
uint64_t ChessLogic::getBishopMoveBitBoard(short color) const {
    uint64_t bishopBitboard = getPieceBitBoard(color, 3) | getPieceBitBoard(color, 5); // Get bitboard for bishops and queens
    uint64_t emptySquares = ~getColorBitBoard(1) & ~getColorBitBoard(2); // Empty squares
    uint64_t opponentBitboard = getColorBitBoard(color == 1 ? 2 : 1); // Opponent pieces

    uint64_t bishopMoves = 0;
    const short bishopOffsets[4] = {-9, -7, 7, 9}; // Diagonal moves};

    for (short i = 63; i >= 0; i--) {
        if (bishopBitboard & (1ULL << i)) { // if bishop is on the square
            for (short j = 0; j < 4; ++j) {
                short targetSquare = i + bishopOffsets[j];
                while (targetSquare >= 0 && targetSquare < 64) {
                    if ((bishopOffsets[j] == -9 && (i % 8 == 0))
                        || (bishopOffsets[j] == -7 && (i % 8 == 7))
                        || (bishopOffsets[j] == 7 && (i % 8 == 0))
                        || (bishopOffsets[j] == 9 && (i % 8 == 7))
                    ) {
                        
                        break; // Skip invalid moves
                    }
                    bishopMoves |= (1ULL << targetSquare);
                    if ((bishopOffsets[j] == -9 && (targetSquare % 8 == 0))
                        || (bishopOffsets[j] == -7 && (targetSquare % 8 == 7))
                        || (bishopOffsets[j] == 7 && (targetSquare % 8 == 0))
                        || (bishopOffsets[j] == 9 && (targetSquare % 8 == 7))
                    ) {
                        // reached a corner
                        
                        break; // Skip invalid moves
                    }
                    targetSquare = targetSquare + bishopOffsets[j];
                    if (internalBoard[targetSquare].type != 0) {
                        break; // Stop at the first piece encountered
                    }
                    
                }
            }
        }
    }

    return (bishopMoves & emptySquares) | opponentBitboard;
}
uint64_t ChessLogic::getRookMoveBitBoard(short color) const {
    uint64_t rookBitboard = getPieceBitBoard(color, 4) | getPieceBitBoard(color, 5); // Get bitboard for rooks and queens
    uint64_t emptySquares = ~getColorBitBoard(1) & ~getColorBitBoard(2); // Empty squares
    uint64_t opponentBitboard = getColorBitBoard(color == 1 ? 2 : 1); // Opponent pieces

    uint64_t rookMoves = 0;
    const short rookOffsets[4] = {-8, 8, -1, 1}; // Straight line moves

    for (short i = 0; i < 64; ++i) {
        if (rookBitboard & (1ULL << i)) {
            for (short j = 0; j < 4; ++j) {
                short targetSquare = i + rookOffsets[j];
                while (targetSquare >= 0 && targetSquare < 64) {
                    if ((rookOffsets[j] == -1 && (i % 8 == 0))
                        || (rookOffsets[j] == 1 && (i % 8 == 7))) {
                        break; // Skip invalid moves
                    }
                    rookMoves |= (1ULL << targetSquare);
                    if ((rookOffsets[j] == -1 && (targetSquare % 8 == 0))
                        || (rookOffsets[j] == 1 && (targetSquare % 8 == 7))) {
                        break; // Skip invalid moves
                    }
                    targetSquare = targetSquare + rookOffsets[j];
                    if (internalBoard[targetSquare].type != 0) {
                        break; // Stop at the first piece encountered
                    }
                }
            }
        }
    }

    return (rookMoves & emptySquares) | opponentBitboard;
}

// method not needed
uint64_t ChessLogic::getQueenMoveBitBoard(short color) const {
    uint64_t queenBitboard = getPieceBitBoard(color, 5); // Get bitboard for queens
    uint64_t emptySquares = ~getColorBitBoard(1) & ~getColorBitBoard(2); // Empty squares
    uint64_t opponentBitboard = getColorBitBoard(color == 1 ? 2 : 1); // Opponent pieces

    uint64_t queenMoves = (getBishopMoveBitBoard(color) | getRookMoveBitBoard(color)) & emptySquares;

    return (getBishopMoveBitBoard(color) | getRookMoveBitBoard(color)) | opponentBitboard;
}

uint64_t ChessLogic::getKingMoveBitBoard(short color) const {
    uint64_t kingBitboard = getPieceBitBoard(color, 6); // Get bitboard for kings
    uint64_t emptySquares = ~getColorBitBoard(1) & ~getColorBitBoard(2); // Empty squares
    uint64_t opponentBitboard = getColorBitBoard(color == 1 ? 2 : 1); // Opponent pieces

    uint64_t kingMoves = 0;
    const short kingOffsets[8][2] = {{-1, -8}, {0, -8}, {1, -8}, {-1, 0}, {1, 0}, {-1, 8}, {0, 8}, {1, 8}};

    for (short i = 63; i >= 0; i--) {
        if (kingBitboard & (1ULL << i)) {
            for (short j = 0; j < 8; ++j) {
                short targetSquare = i + kingOffsets[j][0] + kingOffsets[j][1];
                if (targetSquare >= 0 && targetSquare < 64) {
                    if ((kingOffsets[j][0] == -1 && (i % 8 == 0)) || 
                        (kingOffsets[j][0] == 1 && (i % 8 == 7))) {
                        continue; // Skip invalid moves
                    }
                    kingMoves |= (1ULL << targetSquare);
                }
            }
        }
    }

    return (kingMoves & emptySquares) | opponentBitboard;
}

void ChessLogic::initializeZobrist() {
    std::mt19937_64 rng(123456); // Random number generator with a fixed seed
    std::uniform_int_distribution<uint64_t> dist;

    // Initialize Zobrist table for pieces on squares
    for (int square = 0; square < 64; ++square) {
        for (int piece = 0; piece < 12; ++piece) { // 6 piece types * 2 colors
            zobristTable[square][piece] = dist(rng);
        }
    }

    // Initialize Zobrist values for castling rights
    for (int i = 0; i < 4; ++i) {
        zobristCastling[i] = dist(rng);
    }

    // Initialize Zobrist values for en passant files
    for (int i = 0; i < 8; ++i) {
        zobristEnPassant[i] = dist(rng);
    }

    // Initialize Zobrist value for the player's turn
    zobristTurn = dist(rng);
}

uint64_t ChessLogic::hashPosition(bool isWhiteTurn) const {
    uint64_t hash = 0;

    // Hash the pieces on the board
    for (int square = 0; square < 64; ++square) {
        const chessPiece &piece = internalBoard[square];
        if (piece.type != 0) { // If the square is not empty
            int pieceIndex = (piece.type - 1) + (piece.color - 1) * 6; // Map to 0-11
            hash ^= zobristTable[square][pieceIndex];
        }
    }

    // Hash castling rights
    if (whiteKCastle) hash ^= zobristCastling[0];
    if (whiteQCastle) hash ^= zobristCastling[1];
    if (blackKCastle) hash ^= zobristCastling[2];
    if (blackQCastle) hash ^= zobristCastling[3];

    // Hash en passant square
    if (enPassantSquare != -1) {
        int file = enPassantSquare % 8; // Get the file of the en passant square
        hash ^= zobristEnPassant[file];
    }

    // Hash the player's turn
    if (isWhiteTurn) {
        hash ^= zobristTurn;
    }

    return hash;
}

std::vector<ChessLogic::Move> ChessLogic::getMoveHistory() const {
    std::vector<Move> moveHistory;
    std::stack<Move> tempStack = moveStack;

    while (!tempStack.empty()) {
        moveHistory.push_back(tempStack.top());
        tempStack.pop();
    }
    std::reverse(moveHistory.begin(), moveHistory.end());
    return moveHistory;
}

std::string ChessLogic::printBitBoard(uint64_t bitboard) const {
    std::string bitboardString = std::bitset<64>(bitboard).to_string();
    std::string formattedString = "\n";
    for (int i = 64; i > 0; i--) {
        formattedString += bitboardString[i-1];
        if ((i - 1) % 8 == 0) {
            formattedString += "\n";
        }
    }
    return formattedString;
}

std::string ChessLogic::printBoard() const {
    std::string formattedString = "\n";
    const char pieceRef[] = {'0', 'p', 'n', 'b', 'r', 'q', 'k'};

    for (int i = 0; i < 64; ++i) {
        if (internalBoard[i].type == 0) {
            formattedString += ".";
        } else {
            
            // formattedString += std::to_string(internalBoard[i].type);
            if (internalBoard[i].color ==1) {
                formattedString += toupper(pieceRef[internalBoard[i].type]);
            } else {
                formattedString += pieceRef[internalBoard[i].type];
            }
        }
        formattedString += " ";
        if ((i + 1) % 8 == 0) {
            formattedString += "\n";
        }
    }
    return formattedString;
    
}

std::string ChessLogic::squareToString(short square) const {
    if (square < 0 || square >= 64) {
        return "Invalid square";
    }
    char file = 'a' + (square % 8);
    char rank = '8' - (square / 8);
    return std::string(1, file) + std::string(1, rank);
}

std::string ChessLogic::printLegalMoves(std::vector<ChessLogic::Move> moves) const {
    std::string formattedString = "\nLegal Moves:\n";
    for (const auto &move : moves) {
        formattedString += translateMoveToString(move) + ", ";
    }
    if (!moves.empty()) {
        formattedString = formattedString.substr(0, formattedString.length() - 2); // Remove trailing comma and space
    }
    std::cout << formattedString << std::endl;
    return formattedString;
}

short ChessLogic::stringToSquare(const std::string &squareStr) const {
    if (squareStr.length() != 2) {
        std::cerr << "Error: Invalid square string provided to stringToSquare." << std::endl;
        std::abort();
    }
    char file = squareStr[0];
    char rank = squareStr[1];

    if (file < 'a' || file > 'h' || rank < '1' || rank > '8') {
        std::cerr << "Error: Invalid square string provided to stringToSquare." << std::endl;
        std::abort();
    }
    short square = (('1' - rank) + 7 ) * 8 + (file - 'a');
    return square;
}

std::string ChessLogic::printMoves(std::vector<Move> moves) const {
    std::string moveString;
    for (Move move : moves) {
        moveString += move.color == 1 ? "w: " : "b:";
        moveString += translateMoveToString(move) + ", ";
    }
    if (moveString.length() > 2) {
        moveString.pop_back();
        moveString.pop_back();
    }
    return moveString;
}
