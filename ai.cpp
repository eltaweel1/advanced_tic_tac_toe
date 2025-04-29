#include "ai.h"
#include <random>
#include <vector>
#include <limits>

AI::AI(const QString &difficulty, char aiSymbol, char playerSymbol)
    : difficulty(difficulty), aiSymbol(aiSymbol), playerSymbol(playerSymbol) {}

void AI::makeMove(GameLogic *gameLogic) {
    if (difficulty == "easy") {
        std::vector<std::pair<int, int>> emptyCells;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (gameLogic->getCell(i, j) == ' ')
                    emptyCells.emplace_back(i, j);

        if (emptyCells.empty()) return;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, emptyCells.size() - 1);
        auto [row, col] = emptyCells[dis(gen)];
        gameLogic->makeMove(row, col, aiSymbol);

    } else {
        int depthLimit = (difficulty == "medium") ? 2 : -1; // -1 = لا يوجد حد
        auto [bestRow, bestCol] = findBestMove(gameLogic, depthLimit);
        gameLogic->makeMove(bestRow, bestCol, aiSymbol);
    }
}

std::pair<int, int> AI::findBestMove(GameLogic *gameLogic, int depthLimit) {
    int bestScore = std::numeric_limits<int>::min();
    std::pair<int, int> bestMove = {-1, -1};

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (gameLogic->getCell(i, j) == ' ') {
                gameLogic->makeMove(i, j, aiSymbol);
                int score = minimax(gameLogic, 0, false, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), depthLimit);
                gameLogic->undoMove(i, j);

                if (score > bestScore) {
                    bestScore = score;
                    bestMove = {i, j};
                }
            }
        }
    }
    return bestMove;
}

int AI::minimax(GameLogic *gameLogic, int depth, bool isMaximizing, int alpha, int beta, int depthLimit) {
    if (gameLogic->checkWin(aiSymbol)) return 10 - depth;
    if (gameLogic->checkWin(playerSymbol)) return depth - 10;
    if (gameLogic->checkDraw()) return 0;
    if (depthLimit != -1 && depth >= depthLimit) return evaluateBoard(gameLogic);

    int bestScore = isMaximizing ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (gameLogic->getCell(i, j) == ' ') {
                gameLogic->makeMove(i, j, isMaximizing ? aiSymbol : playerSymbol);
                int score = minimax(gameLogic, depth + 1, !isMaximizing, alpha, beta, depthLimit);
                gameLogic->undoMove(i, j);

                if (isMaximizing) {
                    bestScore = std::max(bestScore, score);
                    alpha = std::max(alpha, score);
                } else {
                    bestScore = std::min(bestScore, score);
                    beta = std::min(beta, score);
                }

                if (beta <= alpha)
                    return bestScore;  // قطع البحث (pruning)
            }
        }
    }

    return bestScore;
}

int AI::evaluateBoard(GameLogic *gameLogic) {
    if (gameLogic->checkWin(aiSymbol)) return 10;
    if (gameLogic->checkWin(playerSymbol)) return -10;
    return 0;
}
