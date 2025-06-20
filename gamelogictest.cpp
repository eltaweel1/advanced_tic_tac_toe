#include "gamelogictest.h"
#include "gamelogic.h"
#include <iostream>

void GameLogicTest::runTests() {
    GameLogic game;

    std::cout << "Running GameLogic Tests...\n";

    // Test 1: Diagonal win
    game.reset();
    game.makeMove(0, 0, 'X');
    game.makeMove(1, 1, 'X');
    game.makeMove(2, 2, 'X');
    if (game.checkWin('X')) {
        std::cout << "✅ Test 1 Passed: Diagonal win detected.\n";
    } else {
        std::cout << "❌ Test 1 Failed: Diagonal win not detected.\n";
    }

    // Test 2: Draw
    game.reset();
    char moves[3][3] = {
        {'X', 'O', 'X'},
        {'X', 'O', 'O'},
        {'O', 'X', 'X'}
    };
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            game.makeMove(i, j, moves[i][j]);
    if (game.checkDraw()) {
        std::cout << "✅ Test 2 Passed: Draw detected.\n";
    } else {
        std::cout << "❌ Test 2 Failed: Draw not detected.\n";
    }

    // Test 3: Invalid move
    game.reset();
    game.makeMove(0, 0, 'X');
    if (!game.makeMove(0, 0, 'O')) {
        std::cout << "✅ Test 3 Passed: Invalid move rejected.\n";
    } else {
        std::cout << "❌ Test 3 Failed: Invalid move accepted.\n";
    }

    // Test 4: Undo move
    game.reset();
    game.makeMove(1, 1, 'O');
    game.undoMove(1, 1);
    if (game.getCell(1, 1) == ' ') {
        std::cout << "✅ Test 4 Passed: Undo move worked.\n";
    } else {
        std::cout << "❌ Test 4 Failed: Undo move failed.\n";
    }

    std::cout << "Tests completed.\n";
}
