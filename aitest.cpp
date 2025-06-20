#include "aitest.h"
#include "ai.h"
#include "gamelogic.h"
#include <QDebug>

// Helper function to check if there are any empty cells left on the board
bool hasMovesLeft(const GameLogic& game) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (game.getCell(i, j) == ' ')  // adjust if your game uses a different empty marker
                return true;
    return false;
}

void AITest::runTests() {
    qDebug() << "\nRunning AI Tests...";

    // === Test 1: AI blocks winning move ===
    {
        GameLogic game;
        game.makeMove(0, 0, 'X');
        game.makeMove(0, 1, 'X');
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

    // === Test 4: isMovesLeft - partially filled board ===
    {
        GameLogic game;
        game.makeMove(0, 0, 'X');
        if (hasMovesLeft(game)) {
            qDebug() << "✅ Test 4 Passed: Detected moves left on partially filled board.";
        } else {
            qDebug() << "❌ Test 4 Failed: Did not detect moves left.";
        }
    }

    // === Test 5: isMovesLeft - full board ===
    {
        GameLogic game;
        char board[3][3] = {
            {'X', 'O', 'X'},
            {'X', 'O', 'O'},
            {'O', 'X', 'X'}
        };
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                game.makeMove(i, j, board[i][j]);

        if (!hasMovesLeft(game)) {
            qDebug() << "✅ Test 5 Passed: Detected full board correctly.";
        } else {
            qDebug() << "❌ Test 5 Failed: Incorrectly detected moves left.";
        }
    }

    // === Test 6: Medium vs Easy AI ===
    {
        int mediumWins = 0;
        int games = 100;

        for (int i = 0; i < games; ++i) {
            GameLogic game;
            AI easy("easy", 'X', 'O');
            AI medium("medium", 'O', 'X');
            char currentPlayer = 'X';

            while (!game.checkWin('X') && !game.checkWin('O') && hasMovesLeft(game)) {
                if (currentPlayer == 'X') {
                    easy.makeMove(&game);
                    currentPlayer = 'O';
                } else {
                    medium.makeMove(&game);
                    currentPlayer = 'X';
                }
            }

            if (game.checkWin('O')) ++mediumWins;
        }

        if (mediumWins > 50) {
            qDebug() << "✅ Test 6 Passed: Medium AI beat Easy AI (" << mediumWins << " wins).";
        } else {
            qDebug() << "❌ Test 6 Failed: Medium AI too weak (" << mediumWins << " wins).";
        }
    }

    // === Test 8: Hard vs Hard AI (should always draw) ===
    {
        int draws = 0;
        int games = 100;

        for (int i = 0; i < games; ++i) {
            GameLogic game;
            AI hard1("hard", 'X', 'O');
            AI hard2("hard", 'O', 'X');
            char currentPlayer = 'X';

            while (!game.checkWin('X') && !game.checkWin('O') && hasMovesLeft(game)) {
                if (currentPlayer == 'X') {
                    hard1.makeMove(&game);
                    currentPlayer = 'O';
                } else {
                    hard2.makeMove(&game);
                    currentPlayer = 'X';
                }
            }

            if (!game.checkWin('X') && !game.checkWin('O')) ++draws;
        }

        if (draws == 100) {
            qDebug() << "✅ Test 7 Passed: Hard AI played perfect game (100 draws).";
        } else {
            qDebug() << "❌ Test 7 Failed: Hard AI made mistakes (" << draws << " draws).";
        }
    }

    qDebug() << "AI Tests completed.";
}
