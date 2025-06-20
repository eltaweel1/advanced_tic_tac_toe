#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <stack>
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
    QPushButton *restartButton;
    QPushButton *returnButton;
    std::stack<QString> moveHistory;
    bool animationsEnabled;
};

#endif // BOARD_H
