Feature: Mate in three

    Scenario: M3 Puzzle 1
        Given FEN "8/1Np1nr2/1p2pr2/1R6/1Pk2bR1/K3p3/2P1N1B1/8 w - - 0 1"
        Then Display the board
        # Then Bot(5) should play "b5b6"
        When The move "b5b6" is played
        When The move "f6f5" is played
        Then Bot(5) should play "g4f4"
        When The move "f5f4" is played
        Then Bot(5) should play "b7a5"

