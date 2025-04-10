Feature: Mate in three

    Scenario: M3 Puzzle 1
        Given FEN "8/1Np1nr2/1p2pr2/1R6/1Pk2bR1/K3p3/2P1N1B1/8 w - - 0 1"
        Given Eval strategy "mat_pos_eval"
        Then Display the board
        Then Bot(9, 10) should play "b5b6" using: (8) threads
        When The move "f6f5" is played
        Then Bot(9, 10) should play "g4f4"
        When The move "f5f4" is played
        Then Bot(9, 10) should play "b7a5" using: (8) threads
        Then The score should be "1 - 0"

    Scenario: M3 backrank mate 1
        Given FEN "1k6/ppp5/8/8/4r3/3r4/8/K6R w - - 0 1"
        Given Eval strategy "material_eval"
        Then Display the board
        Then Bot(7, 3) should play "h1h8"
        When The move "e4e8" is played
        Then Bot(7, 3) should play "h8e8"
        When The move "d3d8" is played
        Then Bot(7, 3) should play "e8d8"
        Then The score should be "1 - 0"
