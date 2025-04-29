#include "gamelogic.h"

GameLogic::GameLogic() {
    reset();
}

bool GameLogic::makeMove(int row, int col, char symbol) {
    if (board[row][col] == ' ') {
        board[row][col] = symbol;
        ++movesMade;
        return true;
    }
    return false;
}

bool GameLogic::checkWin(char symbol) const {
    // صفوف وأعمدة
    for (int i = 0; i < 3; ++i) {
        if ((board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol) ||
            (board[0][i] == symbol && board[1][i] == symbol && board[2][i] == symbol))
            return true;
    }

    // قطريين
    return (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) ||
           (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol);
}

bool GameLogic::checkDraw() const {
    return movesMade == 9 && !checkWin('X') && !checkWin('O');
}

char GameLogic::getCell(int row, int col) const {
    return board[row][col];
}

void GameLogic::reset() {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            board[i][j] = ' ';
    movesMade = 0;
}

void GameLogic::undoMove(int row, int col) {
    if (board[row][col] != ' ') {
        board[row][col] = ' ';
        --movesMade;
    }
}
