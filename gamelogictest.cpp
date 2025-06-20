#include "gamelogictest.h"
#include "gamelogic.h"
#include <iostream>

// char GameLogic::switchPlayer(char currentPlayer) {
//     return (currentPlayer == 'X') ? 'O' : 'X';
// }


void GameLogicTest::runTests() {
    GameLogic game;

    std::cout << "Running GameLogic Tests...\n";

    // Test 1.1: checkWin - all win conditions
    bool passedWin = true;

    // Row win
    game.reset();
    game.makeMove(0, 0, 'X');
    game.makeMove(0, 1, 'X');
    game.makeMove(0, 2, 'X');
    passedWin &= game.checkWin('X');

    // Column win
    game.reset();
    game.makeMove(0, 1, 'O');
    game.makeMove(1, 1, 'O');
    game.makeMove(2, 1, 'O');
    passedWin &= game.checkWin('O');

    // Diagonal win
    game.reset();
    game.makeMove(0, 0, 'X');
    game.makeMove(1, 1, 'X');
    game.makeMove(2, 2, 'X');
    passedWin &= game.checkWin('X');

    // Anti-diagonal win
    game.reset();
    game.makeMove(0, 2, 'O');
    game.makeMove(1, 1, 'O');
    game.makeMove(2, 0, 'O');
    passedWin &= game.checkWin('O');

    // Incomplete win (should not be detected)
    game.reset();
    game.makeMove(0, 0, 'X');
    game.makeMove(0, 1, 'X');
    passedWin &= !game.checkWin('X');

    std::cout << (passedWin ? "✅" : "❌") << " Test 1.1 Passed: checkWin detects all win conditions and avoids false positives.\n";

    // Test 1.2: checkDraw
    bool passedDraw = true;

    // Not draw: still empty cells and a win present
    game.reset();
    game.makeMove(0, 0, 'X');
    game.makeMove(0, 1, 'X');
    game.makeMove(0, 2, 'X');
    passedDraw &= !game.checkDraw();

    // Real draw: full board with no win
    game.reset();
    char drawBoard[3][3] = {
        {'X', 'O', 'X'},
        {'X', 'O', 'O'},
        {'O', 'X', 'X'}
    };
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            game.makeMove(i, j, drawBoard[i][j]);
    passedDraw &= game.checkDraw();

    std::cout << (passedDraw ? "✅" : "❌") << " Test 1.2 Passed: checkDraw recognizes draw and not-draw conditions correctly.\n";

    // Test 1.3: makeMove
    bool passedMove = true;

    // Valid move
    game.reset();
    passedMove &= game.makeMove(2, 2, 'X') && game.getCell(2, 2) == 'X';

    // Invalid move: out-of-bounds
    passedMove &= !game.makeMove(-1, 0, 'O');
    passedMove &= !game.makeMove(3, 3, 'O');

    // Invalid move: overwriting
    passedMove &= !game.makeMove(2, 2, 'O');

    std::cout << (passedMove ? "✅" : "❌") << " Test 1.3 Passed: makeMove validates input, placement, and prevents overwrite.\n";

    std::cout << "All tests completed.\n";
}
