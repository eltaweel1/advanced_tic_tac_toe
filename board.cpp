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
    statusLabel(nullptr),
    restartButton(nullptr),
    returnButton(nullptr),
    animationsEnabled(true)
{
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
    statusLabel = new QLabel(QString("%1's Turn").arg(username), this);
    statusLabel->setAlignment(Qt::AlignCenter);
    QFont statusFont = statusLabel->font();
    statusFont.setPointSize(20);
    statusLabel->setFont(statusFont);
    mainLayout->addWidget(statusLabel);

    // 3x3 game board
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(10);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cells[i][j] = new QPushButton(this);
            cells[i][j]->setFixedSize(100, 100);
            QFont font = cells[i][j]->font();
            font.setPointSize(24);
            cells[i][j]->setFont(font);
            connect(cells[i][j], &QPushButton::clicked, this, &Board::handleCellClick);
            gridLayout->addWidget(cells[i][j], i, j);
        }
    }
    mainLayout->addLayout(gridLayout);

    // Control buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    restartButton = new QPushButton("Restart", this);
    restartButton->setMinimumHeight(50);
    returnButton = new QPushButton("Return to Main Menu", this);
    returnButton->setMinimumHeight(50);
    buttonLayout->addWidget(restartButton);
    buttonLayout->addWidget(returnButton);
    mainLayout->addLayout(buttonLayout);

    // Connect control buttons
    connect(restartButton, &QPushButton::clicked, this, &Board::handleRestart);
    connect(returnButton, &QPushButton::clicked, this, &Board::handleReturn);

    // Apply styling
    setWindowTitle("Tic Tac Toe - Game");
    setStyleSheet(
        "QWidget { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #00695C, stop:1 #4DD0E1); }"
        "QLabel { color: #FFFFFF; font-size: 20px; text-shadow: 1px 1px 2px #000000; }"
        "QPushButton { background: #FF6E40; color: #1A237E; border-radius: 8px; padding: 12px; font-size: 18px; font-weight: bold; }"
        "QPushButton:hover { background: #FF8A65; transform: scale(1.05); }"
        "QPushButton:disabled { background: #CFD8DC; color: #78909C; }"
        "QPushButton[flat='true'] { background: #FFFFFF; color: #1A237E; border: 2px solid #4FC3F7; border-radius: 8px; box-shadow: 2px 2px 4px #00000033; }"
        "QPushButton[flat='true']:hover { background: #B3E5FC; }"
        );

    resize(600, 700);

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

    if (!gameLogic->makeMove(row, col, currentPlayer)) return;

    button->setText(QString(currentPlayer));
    if (animationsEnabled) {
        animateCell(button);
    }

    moveHistory.push(QString("%1,%2,%3").arg(row).arg(col).arg(currentPlayer));

    if (gameLogic->checkWin(currentPlayer)) {
        statusLabel->setText(QString("%1 Wins!").arg(currentPlayer == playerSymbol ? username : "Opponent"));
        saveGameResult(currentPlayer == playerSymbol ? "Win" : "Loss");
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

    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    updateStatus();

    if (mode == "PvE" && currentPlayer == aiSymbol) {
        makeAIMove();
    }
}

void Board::makeAIMove() {
    ai->makeMove(gameLogic);

    int aiRow = -1, aiCol = -1;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            char cell = gameLogic->getCell(i, j);
            cells[i][j]->setText(cell == ' ' ? "" : QString(cell));
            if (cell == aiSymbol) {
                aiRow = i;
                aiCol = j;
            }
        }
    }

    if (animationsEnabled && aiRow != -1 && aiCol != -1) {
        animateCell(cells[aiRow][aiCol]);
    }

    moveHistory.push(QString("%1,%2,%3").arg(aiRow).arg(aiCol).arg(aiSymbol));

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
    if (mode == "PvP") {
        statusLabel->setText(QString("%1's Turn").arg(currentPlayer == playerSymbol ? username : "Opponent"));
    } else if (mode == "PvE") {
        statusLabel->setText(QString("%1's Turn").arg(currentPlayer == playerSymbol ? username : "AI"));
    }
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
    while (!moveHistory.empty()) moveHistory.pop();
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
