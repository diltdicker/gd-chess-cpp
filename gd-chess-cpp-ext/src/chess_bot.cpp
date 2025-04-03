#include "chess_bot.h"

ChessBot::ChessBot() {
    moveStrategy = new BestEvalMoveStrategy();
    evalStrategy = new MatPosEvalStrategy();
    this->setFEN(DEFAULT_FEN);
}

// Destructor that ensures the move strategy is deleted
ChessBot::~ChessBot() {
    if (moveStrategy != nullptr) {
        delete moveStrategy;
        moveStrategy = nullptr;
    }
    if (evalStrategy != nullptr) {
        delete evalStrategy;
        evalStrategy = nullptr;
    }
}

bool ChessBot::validateMove(const std::string &move) {
    // Check if the move is in the correct format (e.g., "e2e4")
    if (move.length() < 4 || move.length() > 5) {
        return false;
    }

    // Check if the first two characters are valid squares
    if (!isalpha(move[0]) || !isdigit(move[1]) || !isalpha(move[2]) || !isdigit(move[3])) {
        return false;
    }

    // Check for optional promotion character
    if (move.length() == 5 && !strchr("nbrq", move[4])) {
        return false;
    }

    if (move[0] < 'a' || move[0] > 'h' || move[2] < 'a' || move[2] > 'h') {
        return false; // Invalid file
    }
    if (move[1] < '1' || move[1] > '8' || move[3] < '1' || move[3] > '8') {
        return false; // Invalid rank
    }

    if (move[0] == move[2] && move[1] == move[3]) {
        return false; // No movement
    }

    if (halfMoveClock >= 100) {
        return false; // 50-move rule
    }

    // Validate the move using the bot logic
    ChessLogic::Move translatedMove = botLogic.translateMove(move);
    return botLogic.isMoveLegal(translatedMove);
}

void ChessBot::applyMove(const std::string &move) {

    // Apply the move using the bot logic
    ChessLogic::Move translatedMove = botLogic.translateMove(move);
    botLogic.makeMove(translatedMove);

    // Update the turn
    isWhiteTurn = !isWhiteTurn;

    // Update the half move clock
    halfMoveClock++;
    if (translatedMove.piece == 1) { // If a pawn is moved

        halfMoveClock = 0; // Reset the half move clock
    }
    if (botLogic.getChessBoard()[translatedMove.to].color != 0) { // If a piece is captured
        halfMoveClock = 0; // Reset the half move clock
    }

    // Update the full move number
    if (!isWhiteTurn) {
        fullMoveNumber++;
    }
}

void ChessBot::setFEN(const std::string &fen) {
    std::regex fenRegex("([0-8prnbqkPRNBQK/]+) ([wb]) ([KQkq-]+) ([0-8a-h-]+) (\\d+) (\\d+)");
    std::smatch match;
    ChessLogic::chessPiece chessBoard[64]; // Initialize the chess board

    if (std::regex_match(fen, match, fenRegex)) {
        std::string board = match[1].str();
        char turn = match[2].str()[0];
        std::string castling = match[3].str();
        std::string enPassant = match[4].str();
        int halfMove = std::stoi(match[5].str());
        int fullMove = std::stoi(match[6].str());

        for (size_t i = 0; i < 64; i++) {
            chessBoard[i] = {0, 0};
        }

        size_t index = 0;
        for (char c : board) {
            if (isdigit(c)) {
                index += c - '0';
            } else {
                short color = (isupper(c)) ? 1 : 2;
                short type = 0;
                switch (tolower(c)) {
                    case 'p': type = 1; break;
                    case 'n': type = 2; break;
                    case 'b': type = 3; break;
                    case 'r': type = 4; break;
                    case 'q': type = 5; break;
                    case 'k': type = 6; break;
                }
                chessBoard[index] = {color, type};
                index++;
            }
        }

        isWhiteTurn = (turn == 'w');
        botLogic.whiteQCastle = castling.find('Q') != std::string::npos;
        botLogic.whiteKCastle = castling.find('K') != std::string::npos;
        botLogic.blackQCastle = castling.find('q') != std::string::npos;
        botLogic.blackKCastle = castling.find('k') != std::string::npos;

        if (enPassant != "-") {
            botLogic.enPassantSquare = (enPassant[0] - 'a') + ((enPassant[1] - '1') * 8);
        } else {
            botLogic.enPassantSquare = -1;
        }

        halfMoveClock = halfMove;
        fullMoveNumber = fullMove;

        // Update the bot logic with the new board state
        botLogic.copyChessBoard(chessBoard);
        botLogic.emtpyMoveStack();

    } else {
        fprintf(stderr, "Invalid FEN string format. Aborting program.\n");
        std::abort();
    }
}

std::string ChessBot::getFEN() const {
    std::string fen;
    const ChessLogic::chessPiece* board = botLogic.getChessBoard();
    const char pieceLookup[7] = {'0', 'p', 'n', 'b', 'r', 'q', 'k'};

    int emptyCount = 0;
    for (int i = 0; i < 64; ++i) {
        if (board[i].type == 0) {
            emptyCount++;
            // fen += '1';
        } else {
            if (emptyCount > 0) {
                fen += std::to_string(emptyCount);
                emptyCount = 0;
            }
            fen += (board[i].color == 1) ? toupper(pieceLookup[board[i].type]) : tolower(pieceLookup[board[i].type]);
        }
        if ((i + 1) % 8 == 0 && i != 63) {
            if (emptyCount > 0) {
                fen += std::to_string(emptyCount);
                emptyCount = 0;
            }
            fen += '/';
        } else if (i == 63 && emptyCount > 0) {
            fen += std::to_string(emptyCount);
        }
    }

    fen += " ";
    fen += (isWhiteTurn ? " w " : " b ");
    fen += " ";
    fen += std::string(botLogic.whiteKCastle ? "K" : "-") +
       std::string(botLogic.whiteQCastle ? "Q" : "-") +
       std::string(botLogic.blackKCastle ? "k" : "-") +
       std::string(botLogic.whiteQCastle ? "q" : "-");
    fen += " ";
    fen += (botLogic.enPassantSquare != -1) ? std::to_string(botLogic.enPassantSquare) : "-";
    fen += " ";
    fen += std::to_string(halfMoveClock) + " " + std::to_string(fullMoveNumber);

    return fen;
}


ChessLogic::Move ChessBot::iterativeDeepeningSearch(short searchDepth, std::chrono::time_point<std::chrono::steady_clock> stopTime) {
    ChessLogic::Move bestMove = ChessLogic::Move();
    botLogic.transpositionTable.clear(); // Clear the transposition table before each search

    for (short depth = 1; depth <= searchDepth; ++depth) {
        bestMove = moveStrategy->getBestMove(botLogic, evalStrategy, isWhiteTurn, depth, stopTime);
        if (std::chrono::steady_clock::now() >= stopTime) {
            break; // Stop if time limit is reached
        }
    }

    return bestMove;
}

bool ChessBot::isCheck() const {
    return botLogic.isInCheck(isWhiteTurn);
}

bool ChessBot::isCheckMate() {
    return botLogic.isInCheck(isWhiteTurn) && botLogic.getLegalMoves(isWhiteTurn).empty();
}
