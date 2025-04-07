Feature: Mate in two

    # Scenario: Backrank mate ver 1

        # Given FEN "1k2r3/8/8/8/8/4r3/PPP5/1K5R b - - 0 1"
        # Then Display the board
        # Then Bot(3) should play "e3e1"
        # When The move "h1e1" is played
        # Then Bot(3) should play "e8e1"

    Scenario: Daily Puzzle trip to the light squares

        Given FEN "1k1r3r/2p1qppp/1pB2b2/p1pP1b2/4p3/4P2P/PPP3P1/1K1R1Q1R w - - 0 1"
        # Given FEN "1k2r3/8/8/8/8/4r3/PPP5/1K5R b - - 0 1"
        Then Display the board
        Then Bot(7) should play "f1a6"
        When The move "f5c8" is played
        Then Bot(7) should play "a6a8"
