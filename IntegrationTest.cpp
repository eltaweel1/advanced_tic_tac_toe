#include "IntegrationTest.h"
#include "gamelogic.h"
#include <iostream>
#include <cassert>

void IntegrationTest::run() {
    const int games = 5;
    const int moves_per_game = 18;

    int moves[5][18] = {
        {1,1, 0,2, 2,0, 0,1, 1,2, 2,1, 0,0, 1,0, 2,2}, // Game 1 - X
        {1,0, 0,0, 1,1, 0,1, 1,2, 2,2, 2,0, 0,2, 2,1}, // Game 2 - X
        {1,1, 2,2, 1,1, 2,0, 2,0, 1,0, 1,0, 1,0, 1,0}, // Game 3 - Draw
        {1,1, 0,0, 2,2, 0,1, 1,2, 0,2, 2,0, 2,1, 1,0}, // Game 4 - O
        {0,1, 0,0, 1,1, 1,0, 2,2, 2,0, 0,2, 2,1, 1,2}  // Game 5 - O wins
    };

    // Adjusted winners based on your comments
    char expectedWinners[games] = {'X', 'X', '-', 'O', 'O'};

    for (int g = 0; g < games; ++g) {
        GameLogic game;
        char current = 'X';
        char winner = '-';

        for (int m = 0; m < moves_per_game; m += 2) {
            int row = moves[g][m];
            int col = moves[g][m + 1];

            if (game.getCell(row, col) != ' ') continue; // Skip if already occupied

            game.makeMove(row, col, current);

            if (game.checkWin(current)) {
                winner = current;
                break;
            }

            if (game.checkDraw()) break;

            current = (current == 'X') ? 'O' : 'X';
        }

        std::cout << "Game " << g + 1 << " result: ";
        if (winner == '-') {
            std::cout << "Draw" << std::endl;
            assert(expectedWinners[g] == '-');
        } else {
            std::cout << winner << " wins" << std::endl;
            assert(expectedWinners[g] == winner);
        }
    }

    std::cout << "✅ All integration tests passed!" << std::endl;
}
