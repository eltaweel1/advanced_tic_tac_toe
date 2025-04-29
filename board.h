#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QStringList>
#include <QLabel>
#include <QPushButton>
#include "gamelogic.h"
#include "ai.h"
#include "usermanager.h"

class QPropertyAnimation;

class Board : public QWidget {
    Q_OBJECT

public:
    Board(const QString &username, char playerSymbol, QString mode, QString aiDifficulty, UserManager *userManager, QWidget *parent = nullptr);
    ~Board();

    QString getBoardState();

signals:
    void gameFinished();
    void returnToMainRequested();

private slots:
    void handleCellClick();
    void makeAIMove();
    void updateStatus();
    void handleUndo();
    void handleRedo();
    void handleRestart();
    void handleReturn();
    void animateCell(QPushButton *button);

private:
    void saveGameResult(const QString &result);
    void disableBoard();

    QString username;
    UserManager *userManager;
    QString mode;
    char playerSymbol;
    char aiSymbol;
    char currentPlayer;
    GameLogic *gameLogic;
    AI *ai;
    QString aiDifficulty;
    QPushButton *cells[3][3];
    QLabel *statusLabel;
    QPushButton *undoButton;
    QPushButton *redoButton;
    QPushButton *restartButton;
    QPushButton *returnButton;
    QStringList moveHistory;
    int historyIndex;
    bool animationsEnabled;
};

#endif // BOARD_H
