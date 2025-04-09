Feature: Chess Mate in One

    Scenario: Fool's Mate
        Given FEN "rnbqkbnr/pppp1ppp/4p3/8/5PP1/8/PPPPP2P/RNBQKBNR b KQkq g3 0 2"
        Then Display the board
        Then Bot(3, 1) should play "d8h4"
        Then The score should be "0 - 1"