Feature: Mate in two

    Scenario: Backrank mate ver 1

        Given FEN "1k2r3/8/8/8/8/4r3/PPP5/1K5R b - - 0 1"
        Then Display the board
        Then Bot(3) should play "e3e1"
        When The move "h1e1" is played
        Then Bot(3) should play "e8e1"

