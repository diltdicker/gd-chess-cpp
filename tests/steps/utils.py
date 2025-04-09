from behave import given, then, when
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

@given('Eval strategy "{strategy}"')
def given_eval_strategy(context, strategy):
    """
    Set the Chessbot with alternative strategy configuration
    """
    context.bot.set_option('eval_strategy', strategy)

@then('Display the board')
def then_display_board(context):
    """
    Display the current state of the chessboard.
    """
    print("Current board state:")
    print(context.board)
    print("----------------")

@then('Bot({depth},{seconds}) should play "{move}"')
def then_bot_move_d_s(context, depth, seconds, move):
    """
    Verify that the bot plays the expected move.
    """
    bot_move = context.bot.get_bot_move(int(depth), 1_000 * int(seconds))
    fen = context.bot.export_fen()
    context.board = Board(fen)

    print(f"Bot move: {bot_move}")
    
    context.bot.make_move(bot_move)
    fen = context.bot.export_fen()
    context.board = Board(fen)
    print(context.board)
    print(f"move: {bot_move}")
    assert bot_move == move, f"Expected move: {move}, but got: {bot_move}"

@when('The move "{move}" is played')
def move_is_played(context, move):

    context.bot.make_move(move)
    fen = context.bot.export_fen()
    context.board = Board(fen)
    print(context.board)
    print(f'move: {move}')

@then('The score should be "{score}"')
def compare_score(context, score):
    game_result = context.bot.get_game_result()
    assert game_result == score, f"unexpected game result is: {game_result}"