#ifndef AI_H
#define AI_H

#include "gamelogic.h"
#include <QString>
#include <utility>
#include <vector>

class AI {
public:
    AI(const QString &difficulty, char aiSymbol, char playerSymbol);
    void makeMove(GameLogic *gameLogic);

    // Getters for testing purposes
    QString getDifficulty() const { return difficulty; }
    char getAiSymbol() const { return aiSymbol; }
    char getPlayerSymbol() const { return playerSymbol; }

private:
    QString difficulty;
    char aiSymbol;
    char playerSymbol;

    // Core AI methods
    std::pair<int, int> findBestMove(GameLogic *gameLogic, int depthLimit = -1);
    int minimax(GameLogic *gameLogic, int depth, bool isMaximizing, int alpha, int beta, int depthLimit);
    int evaluateBoard(GameLogic *gameLogic);

    // Difficulty-specific methods
    void makeRandomMove(GameLogic *gameLogic);
    void makeEasyMove(GameLogic *gameLogic);
    void makeMediumMove(GameLogic *gameLogic);
    void makeHardMove(GameLogic *gameLogic);

    // Utility methods
    std::vector<std::pair<int, int>> getEmptyCells(GameLogic *gameLogic);
    bool isValidMove(GameLogic *gameLogic, int row, int col);
    std::pair<int, int> findImmediateWinOrBlock(GameLogic *gameLogic, char symbol);
    int evaluateLine(char cell1, char cell2, char cell3);
};

#endif // AI_H
