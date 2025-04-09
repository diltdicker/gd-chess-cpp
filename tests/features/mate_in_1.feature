Feature: Chess Mate in One

    Scenario: Fool's Mate
        Given FEN "rnbqkbnr/pppp1ppp/4p3/8/5PP1/8/PPPPP2P/RNBQKBNR b KQkq g3 0 2"
        Then Display the board
        Then Bot(3, 1) should play "d8h4"


# Debug: allowed moves: b:b8a6, b:b8c6, b:d8e7, b:d8f6, b:d8g5, b:d8h4, b:e8e7, b:f8e7, b:f8d6, b:f8c5, b:f8b4, b:f8a3, b:g8e7, b:g8f6, b:g8h6, b:a7a6, b:a7a5, b:b7b6, b:b7b5, b:c7c6, b:c7c5, b:d7d6, b:d7d5, b:f7f6, b:f7f5, b:g7g6, b:g7g5, b:h7h6, b:h7h5, b:e6e5