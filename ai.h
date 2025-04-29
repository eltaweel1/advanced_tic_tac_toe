#ifndef AI_H
#define AI_H

#include "gamelogic.h"
#include <QString>
#include <utility>

class AI {
public:
    AI(const QString &difficulty, char aiSymbol, char playerSymbol);
    void makeMove(GameLogic *gameLogic);

private:
    QString difficulty;
    char aiSymbol;
    char playerSymbol;

    std::pair<int, int> findBestMove(GameLogic *gameLogic, int depthLimit = -1);
    int minimax(GameLogic *gameLogic, int depth, bool isMaximizing, int alpha, int beta, int depthLimit);
    int evaluateBoard(GameLogic *gameLogic);
};

#endif // AI_H
