Feature: Chess Mate in One

    Scenario: Fool's Mate
        Given FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
        # Given FEN "4k3/8/6Q1/8/8/8/8/3K4 w - - 0 1"
        # Given FEN "rnbqkbnr/pppp1ppp/4p3/8/5PP1/8/PPPPP2P/RNBQKBNR b KQkq g3 0 2"
        Then Display the board
        Then Bot should play "d8h4"
