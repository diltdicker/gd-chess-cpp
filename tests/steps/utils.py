from behave import given, then
from gd_chess_bot import GDChessBot
from chess import Board, Move

@given('FEN "{fen}"')
def given_fen(context, fen):
    """
    Set up the chessboard state using the provided FEN string.
    """
    context.bot = GDChessBot()
    context.bot.input_fen(fen)
    context.board = Board(fen)

@then('Display the board')
def then_display_board(context):
    """
    Display the current state of the chessboard.
    """
    print("Current board state:")
    print(context.board)

@then('Bot should play "{move}"')
def then_bot_move(context, move):
    """
    Verify that the bot plays the expected move.
    """
    bot_move = context.bot.get_bot_move(12, 1000)
    print(f"Bot move: {bot_move}")
    uci_moves = [m.uci() for m in context.board.legal_moves]
    if Move.from_uci(bot_move) in uci_moves:
        context.board.push(move)
        context.bot.make_move(move)
        if bot_move != move:
            print("Current board state:")
            print(context.board)
    else:
        print(f"Invalid move: {move}")
        print(Move.from_uci(bot_move))
        print(uci_moves)
        print(context.bot.export_fen())
        # context.bot.debug_bot()
    assert bot_move == move, f"Expected move: {move}, but got: {bot_move}"