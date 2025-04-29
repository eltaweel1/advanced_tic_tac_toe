#include "board.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDateTime>
#include <QPropertyAnimation>

Board::Board(const QString &username, char playerSymbol, QString mode, QString aiDifficulty, UserManager *userManager, QWidget *parent)
    : QWidget(parent),
    username(username),
    userManager(userManager),
    mode(mode),
    playerSymbol(playerSymbol),
    aiSymbol(playerSymbol == 'X' ? 'O' : 'X'),
    currentPlayer('X'),
    gameLogic(new GameLogic()),
    ai(nullptr),
    aiDifficulty(aiDifficulty),
    historyIndex(-1),
    animationsEnabled(true),
    statusLabel(nullptr),
    undoButton(nullptr),
    redoButton(nullptr),
    restartButton(nullptr),
    returnButton(nullptr) {
    // Initialize the cells array
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cells[i][j] = nullptr;
        }
    }

    // Initialize AI for PvE mode
    if (mode == "PvE") {
        ai = new AI(aiDifficulty, aiSymbol, playerSymbol);
    }

    // Set up the UI
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Status label
    statusLabel = new QLabel(QString("Player %1's Turn").arg(currentPlayer), this);
    statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statusLabel);

    // 3x3 game board
    QGridLayout *gridLayout = new QGridLayout();
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cells[i][j] = new QPushButton(this);
            cells[i][j]->setFixedSize(60, 60);
            QFont font = cells[i][j]->font();
            font.setPointSize(20);
            cells[i][j]->setFont(font);
            connect(cells[i][j], &QPushButton::clicked, this, &Board::handleCellClick);
            gridLayout->addWidget(cells[i][j], i, j);
        }
    }
    mainLayout->addLayout(gridLayout);

    // Control buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    undoButton = new QPushButton("Undo", this);
    redoButton = new QPushButton("Redo", this);
    restartButton = new QPushButton("Restart", this);
    returnButton = new QPushButton("Return to Main Menu", this);
    buttonLayout->addWidget(undoButton);
    buttonLayout->addWidget(redoButton);
    buttonLayout->addWidget(restartButton);
    buttonLayout->addWidget(returnButton);
    mainLayout->addLayout(buttonLayout);

    // Connect control buttons
    connect(undoButton, &QPushButton::clicked, this, &Board::handleUndo);
    connect(redoButton, &QPushButton::clicked, this, &Board::handleRedo);
    connect(restartButton, &QPushButton::clicked, this, &Board::handleRestart);
    connect(returnButton, &QPushButton::clicked, this, &Board::handleReturn);

    // Apply styling
    setWindowTitle("Tic Tac Toe - Game");
    setStyleSheet(
        "QWidget { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1565C0, stop:1 #42A5F5); }"
        "QLabel { color: #FFFFFF; font-size: 16px; }"
        "QPushButton { background: #42A5F5; color: #212121; border-radius: 5px; padding: 8px; font-size: 14px; }"
        "QPushButton:hover { background: #64B5F6; }"
        "QPushButton:disabled { background: #B0BEC5; }"
        "QPushButton[flat='true'] { background: transparent; border: 1px solid #BBDEFB; }"
        );

    // AI makes the first move if it's their turn
    if (mode == "PvE" && currentPlayer == aiSymbol) {
        makeAIMove();
    }
}

Board::~Board() {
    delete gameLogic;
    delete ai;
}

void Board::handleCellClick() {
    // Identify the clicked cell
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    int row = -1, col = -1;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (cells[i][j] == button) {
                row = i;
                col = j;
                break;
            }
        }
    }

    // Make the move if valid
    if (!gameLogic->makeMove(row, col, currentPlayer)) return;

    // Update UI and animate
    button->setText(QString(currentPlayer));
    if (animationsEnabled) {
        animateCell(button);
    }

    // Update move history
    moveHistory = moveHistory.mid(0, historyIndex + 1);
    moveHistory.append(QString("%1,%2,%3").arg(row).arg(col).arg(currentPlayer));
    historyIndex++;

    // Check for game end
    if (gameLogic->checkWin(currentPlayer)) {
        statusLabel->setText(QString("Player %1 Wins!").arg(currentPlayer));
        saveGameResult("Win");
        disableBoard();
        emit gameFinished();
        return;
    }
    if (gameLogic->checkDraw()) {
        statusLabel->setText("Draw!");
        saveGameResult("Draw");
        emit gameFinished();
        return;
    }

    // Switch players
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    updateStatus();

    // AI's turn in PvE mode
    if (mode == "PvE" && currentPlayer == aiSymbol) {
        makeAIMove();
    }
}

void Board::makeAIMove() {
    // AI makes a move
    ai->makeMove(gameLogic);

    // Update the board UI
    int aiRow = -1, aiCol = -1;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            char cell = gameLogic->getCell(i, j);
            cells[i][j]->setText(cell == ' ' ? "" : QString(cell));
            // Find the AI's move by checking the last move in history
            if (!moveHistory.isEmpty()) {
                QString lastMove = moveHistory.last();
                QStringList parts = lastMove.split(",");
                int lastRow = parts[0].toInt();
                int lastCol = parts[1].toInt();
                if (i == lastRow && j == lastCol && cell == aiSymbol) {
                    aiRow = i;
                    aiCol = j;
                }
            } else if (cell == aiSymbol) {
                aiRow = i;
                aiCol = j;
            }
        }
    }

    // Animate AI's move
    if (animationsEnabled && aiRow != -1 && aiCol != -1) {
        animateCell(cells[aiRow][aiCol]);
    }

    // Record AI's move (if not already recorded)
    if (aiRow != -1 && aiCol != -1) {
        moveHistory.append(QString("%1,%2,%3").arg(aiRow).arg(aiCol).arg(aiSymbol));
        historyIndex++;
    }

    // Check for game end
    if (gameLogic->checkWin(aiSymbol)) {
        statusLabel->setText("AI Wins!");
        saveGameResult("Loss");
        disableBoard();
        emit gameFinished();
        return;
    }
    if (gameLogic->checkDraw()) {
        statusLabel->setText("Draw!");
        saveGameResult("Draw");
        emit gameFinished();
        return;
    }

    // Switch back to player
    currentPlayer = playerSymbol;
    updateStatus();
}

QString Board::getBoardState() {
    QString state;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            char cell = gameLogic->getCell(i, j);
            state += (cell == ' ' ? '-' : cell);
        }
    }
    return state;
}

void Board::saveGameResult(const QString &result) {
    QString boardState = getBoardState();
    userManager->addGameHistory(username, mode, result, boardState, QDateTime::currentDateTime());
}

void Board::disableBoard() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cells[i][j]->setEnabled(false);
        }
    }
}

void Board::updateStatus() {
    statusLabel->setText(QString("Player %1's Turn").arg(currentPlayer));
    undoButton->setEnabled(historyIndex >= 0);
    redoButton->setEnabled(historyIndex < moveHistory.size() - 1);
}

void Board::handleUndo() {
    if (historyIndex < 0) return;

    QString move = moveHistory[historyIndex];
    QStringList parts = move.split(",");
    int row = parts[0].toInt();
    int col = parts[1].toInt();

    gameLogic->makeMove(row, col, ' ');
    cells[row][col]->setText("");
    historyIndex--;
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    updateStatus();
}

void Board::handleRedo() {
    if (historyIndex >= moveHistory.size() - 1) return;

    historyIndex++;
    QString move = moveHistory[historyIndex];
    QStringList parts = move.split(",");
    int row = parts[0].toInt();
    int col = parts[1].toInt();
    char symbol = parts[2].toUtf8()[0];

    gameLogic->makeMove(row, col, symbol);
    cells[row][col]->setText(QString(symbol));
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    updateStatus();
}

void Board::handleRestart() {
    gameLogic->reset();
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cells[i][j]->setText("");
            cells[i][j]->setEnabled(true);
        }
    }
    currentPlayer = 'X';
    moveHistory.clear();
    historyIndex = -1;
    updateStatus();

    if (mode == "PvE" && currentPlayer == aiSymbol) {
        makeAIMove();
    }
}

void Board::handleReturn() {
    emit returnToMainRequested();
}

void Board::animateCell(QPushButton *button) {
    QPropertyAnimation *animation = new QPropertyAnimation(button, "opacity");
    animation->setDuration(300);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}
