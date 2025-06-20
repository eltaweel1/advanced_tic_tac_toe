#include "aitest.h"
#include "ai.h"
#include "gamelogic.h"
#include <QDebug>

void AITest::runTests() {
    qDebug() << "\nRunning AI Tests...";

    // === Test 1: AI blocks winning move ===
    {
        GameLogic game;
        game.makeMove(0, 0, 'X');
        game.makeMove(0, 1, 'X');
        // 'X' is about to win at (0,2)

        AI ai("hard", 'O', 'X');
        ai.makeMove(&game);

        if (game.getCell(0, 2) == 'O') {
            qDebug() << "✅ Test 1 Passed: AI blocked winning move.";
        } else {
            qDebug() << "❌ Test 1 Failed: AI did not block winning move.";
        }
    }

    // === Test 2: AI wins when it can ===
    {
        GameLogic game;
        game.makeMove(1, 0, 'O');
        game.makeMove(1, 1, 'O');
        game.makeMove(0, 0, 'X');
        game.makeMove(2, 2, 'X');
        // 'O' can win by moving to (1,2)

        AI ai("hard", 'O', 'X');
        ai.makeMove(&game);

        if (game.getCell(1, 2) == 'O' && game.checkWin('O')) {
            qDebug() << "✅ Test 2 Passed: AI made winning move.";
        } else {
            qDebug() << "❌ Test 2 Failed: AI missed winning move.";
        }
    }

    // === Test 3: AI places in empty cell (sanity test) ===
    {
        GameLogic game;
        AI ai("easy", 'O', 'X');
        ai.makeMove(&game);
        bool moveMade = false;
        for (int i = 0; i < 3 && !moveMade; ++i)
            for (int j = 0; j < 3; ++j)
                if (game.getCell(i, j) == 'O')
                    moveMade = true;

        if (moveMade) {
            qDebug() << "✅ Test 3 Passed: AI made a move.";
        } else {
            qDebug() << "❌ Test 3 Failed: AI made no move.";
        }
    }

    qDebug() << "AI Tests completed.";
}
