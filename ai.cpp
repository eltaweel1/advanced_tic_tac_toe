#include "ai.h"
#include <random>
#include <vector>
#include <limits>
#include <algorithm>
#include <stdexcept>

AI::AI(const QString &difficulty, char aiSymbol, char playerSymbol)
    : aiSymbol(aiSymbol), playerSymbol(playerSymbol) {
    // Normalize the difficulty string (trim whitespace and convert to lowercase)
    QString normalizedDifficulty = difficulty.trimmed().toLower();
    
    // Validate and set difficulty
    if (normalizedDifficulty == "easy" || normalizedDifficulty == "medium" || normalizedDifficulty == "hard") {
        this->difficulty = normalizedDifficulty;
    } else {
        // Default to medium if invalid difficulty is provided
        this->difficulty = "medium";
        // Optional: You can uncomment the line below for debugging
        // qDebug() << "Warning: Invalid difficulty '" << difficulty << "'. Defaulting to 'medium'";
    }
    
    if (aiSymbol == playerSymbol) {
        throw std::invalid_argument("AI symbol and player symbol must be different");
    }
}

void AI::makeMove(GameLogic *gameLogic) {
    if (!gameLogic) {
        throw std::invalid_argument("GameLogic pointer cannot be null");
    }

    // Check if there are any available moves
    std::vector<std::pair<int, int>> emptyCells = getEmptyCells(gameLogic);
    if (emptyCells.empty()) {
        return; // No moves available
    }

    QString normalizedDifficulty = difficulty.toLower().trimmed();
    
    if (normalizedDifficulty == "easy") {
        makeEasyMove(gameLogic);
    } else if (normalizedDifficulty == "medium") {
        makeMediumMove(gameLogic);
    } else { // hard or any other value defaults to hard
        makeHardMove(gameLogic);
    }
}

void AI::makeEasyMove(GameLogic *gameLogic) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> chance(1, 100);
    
    // Easy AI: 70% random moves, 30% decent moves
    if (chance(gen) <= 70) {
        makeRandomMove(gameLogic);
    } else {
        // Make a somewhat decent move with very limited depth
        auto [bestRow, bestCol] = findBestMove(gameLogic, 1);
        if (bestRow != -1 && bestCol != -1) {
            gameLogic->makeMove(bestRow, bestCol, aiSymbol);
        } else {
            makeRandomMove(gameLogic); // Fallback to random
        }
    }
}

void AI::makeMediumMove(GameLogic *gameLogic) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> chance(1, 100);
    
    // Medium AI: Always block/win immediate threats, otherwise 60% good moves, 40% random
    auto [winRow, winCol] = findImmediateWinOrBlock(gameLogic, aiSymbol);
    if (winRow != -1 && winCol != -1) {
        gameLogic->makeMove(winRow, winCol, aiSymbol);
        return;
    }
    
    auto [blockRow, blockCol] = findImmediateWinOrBlock(gameLogic, playerSymbol);
    if (blockRow != -1 && blockCol != -1) {
        gameLogic->makeMove(blockRow, blockCol, aiSymbol);
        return;
    }
    
    if (chance(gen) <= 60) {
        // Make a good move with limited depth
        auto [bestRow, bestCol] = findBestMove(gameLogic, 3);
        if (bestRow != -1 && bestCol != -1) {
            gameLogic->makeMove(bestRow, bestCol, aiSymbol);
        } else {
            makeRandomMove(gameLogic);
        }
    } else {
        makeRandomMove(gameLogic);
    }
}

void AI::makeHardMove(GameLogic *gameLogic) {
    // Hard AI: Always optimal play
    auto [bestRow, bestCol] = findBestMove(gameLogic, -1);
    if (bestRow != -1 && bestCol != -1) {
        gameLogic->makeMove(bestRow, bestCol, aiSymbol);
    } else {
        // This should rarely happen, but fallback to random if needed
        makeRandomMove(gameLogic);
    }
}

void AI::makeRandomMove(GameLogic *gameLogic) {
    std::vector<std::pair<int, int>> emptyCells = getEmptyCells(gameLogic);
    if (emptyCells.empty()) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, emptyCells.size() - 1);
    auto [row, col] = emptyCells[dis(gen)];
    gameLogic->makeMove(row, col, aiSymbol);
}

std::pair<int, int> AI::findBestMove(GameLogic *gameLogic, int depthLimit) {
    int bestScore = std::numeric_limits<int>::min();
    std::pair<int, int> bestMove = {-1, -1};

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (gameLogic->getCell(i, j) == ' ') {
                gameLogic->makeMove(i, j, aiSymbol);
                int score = minimax(gameLogic, 0, false, 
                                  std::numeric_limits<int>::min(), 
                                  std::numeric_limits<int>::max(), 
                                  depthLimit);
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
    // Terminal states
    if (gameLogic->checkWin(aiSymbol)) return 10 - depth;
    if (gameLogic->checkWin(playerSymbol)) return depth - 10;
    if (gameLogic->checkDraw()) return 0;
    
    // Depth limit reached
    if (depthLimit != -1 && depth >= depthLimit) {
        return evaluateBoard(gameLogic);
    }

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

                // Alpha-beta pruning
                if (beta <= alpha) {
                    return bestScore;
                }
            }
        }
    }

    return bestScore;
}

int AI::evaluateBoard(GameLogic *gameLogic) {
    if (gameLogic->checkWin(aiSymbol)) return 10;
    if (gameLogic->checkWin(playerSymbol)) return -10;
    
    // Simple heuristic for non-terminal positions
    int score = 0;
    
    // Check rows, columns, and diagonals for potential wins
    for (int i = 0; i < 3; ++i) {
        // Check rows
        score += evaluateLine(gameLogic->getCell(i, 0), gameLogic->getCell(i, 1), gameLogic->getCell(i, 2));
        // Check columns
        score += evaluateLine(gameLogic->getCell(0, i), gameLogic->getCell(1, i), gameLogic->getCell(2, i));
    }
    
    // Check diagonals
    score += evaluateLine(gameLogic->getCell(0, 0), gameLogic->getCell(1, 1), gameLogic->getCell(2, 2));
    score += evaluateLine(gameLogic->getCell(0, 2), gameLogic->getCell(1, 1), gameLogic->getCell(2, 0));
    
    return score;
}

int AI::evaluateLine(char cell1, char cell2, char cell3) {
    int aiCount = 0, playerCount = 0, emptyCount = 0;
    
    if (cell1 == aiSymbol) aiCount++;
    else if (cell1 == playerSymbol) playerCount++;
    else emptyCount++;
    
    if (cell2 == aiSymbol) aiCount++;
    else if (cell2 == playerSymbol) playerCount++;
    else emptyCount++;
    
    if (cell3 == aiSymbol) aiCount++;
    else if (cell3 == playerSymbol) playerCount++;
    else emptyCount++;
    
    // If both players have pieces in the same line, it's neutral
    if (aiCount > 0 && playerCount > 0) return 0;
    
    // Score based on AI advantage
    if (aiCount == 2 && emptyCount == 1) return 5;
    if (aiCount == 1 && emptyCount == 2) return 1;
    if (playerCount == 2 && emptyCount == 1) return -5;
    if (playerCount == 1 && emptyCount == 2) return -1;
    
    return 0;
}

std::vector<std::pair<int, int>> AI::getEmptyCells(GameLogic *gameLogic) {
    std::vector<std::pair<int, int>> emptyCells;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (gameLogic->getCell(i, j) == ' ') {
                emptyCells.emplace_back(i, j);
            }
        }
    }
    return emptyCells;
}

bool AI::isValidMove(GameLogic *gameLogic, int row, int col) {
    return row >= 0 && row < 3 && col >= 0 && col < 3 && gameLogic->getCell(row, col) == ' ';
}

std::pair<int, int> AI::findImmediateWinOrBlock(GameLogic *gameLogic, char symbol) {
    // Check all empty cells to see if placing 'symbol' there would win
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (gameLogic->getCell(i, j) == ' ') {
                gameLogic->makeMove(i, j, symbol);
                if (gameLogic->checkWin(symbol)) {
                    gameLogic->undoMove(i, j);
                    return {i, j};
                }
                gameLogic->undoMove(i, j);
            }
        }
    }
    return {-1, -1}; // No immediate win/block found
}
