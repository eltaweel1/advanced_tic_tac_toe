#include "DatabaseTest.h"
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QtGlobal>

DatabaseTest::DatabaseTest(UserManager *userManager) : userManager(userManager) {}

void DatabaseTest::runAllTests() {
    testUserAllocation();
    testGameHistoryRetrieval();
    qDebug() << "All tests completed.";
}

void DatabaseTest::resetUserManager() {
    userManager->clearUsers();
    QFile userFile("users.txt");
    if (userFile.exists()) {
        userFile.remove();
    }
}

void DatabaseTest::testUserAllocation() {
    qDebug() << "Test 3.1: Database Correct Allocation";
    resetUserManager();
    QString username = "testuser";
    QString password = "testpass";

    qDebug() << "  Register user:" << (userManager->registerUser(username, password) ? "SUCCESS" : "FAILED");
    qDebug() << "  Validate user (valid username, password):" << (userManager->validateUser(username, password) ? "SUCCESS" : "FAILED");
    qDebug() << "  Validate user (invalid username, valid password):" << (!userManager->validateUser("invaliduser", password) ? "SUCCESS" : "FAILED");
    qDebug() << "  Validate user (valid username, invalid password):" << (!userManager->validateUser(username, "invalidpass") ? "SUCCESS" : "FAILED");
    qDebug() << "  Validate user (invalid username, invalid password):" << (!userManager->validateUser("invaliduser", "invalidpass") ? "SUCCESS" : "FAILED");
}

void DatabaseTest::testGameHistoryRetrieval() {
    qDebug() << "Test 3.2: Game History Retrieval";
    resetUserManager();
    userManager->getGameHistory().clear(); // Clear gameHistory first
    QFile historyFile("history.txt");
    if (historyFile.exists()) {
        historyFile.remove();
    }

    struct GameData {
        QString username;
        QString opponent;
        QString result;
        QString boardState;
    };

    QList<GameData> games = {
        {"user1", "user2", "Win",  "XOX-OX-XO"},
        {"user1", "AI",    "Loss", "OXX-XOO--"},
        {"user2", "user1", "Draw", "XOX-OXO-X"},
        {"user1", "AI",    "Win",  "XXX-OO---"},
        {"user2", "user1", "Loss", "OOX-XXO--"},
        {"user1", "AI",    "Draw", "XOX-OXO-X"},
        {"user1", "user2", "Win",  "X-X-O-O-X"},
        {"user2", "AI",    "Loss", "OXX-XOO--"}
    };

    bool usersRegistered = userManager->registerUser("user1", "pass1") && userManager->registerUser("user2", "pass2");
    qDebug() << "  Register users for game history:" << (usersRegistered ? "SUCCESS" : "FAILED");
    if (!usersRegistered) return;

    QDateTime baseTime = QDateTime::currentDateTime();
    for (int i = 0; i < games.size(); ++i) {
        const auto &game = games[i];
        QString mode = game.opponent == "AI" ? "PvE" : "PvP";
        userManager->addGameHistory(game.username, mode, game.result, game.boardState, baseTime.addSecs(i));
    }

    QList<Game> history = userManager->getGameHistory();
    if (history.size() != games.size()) {
        qDebug() << "  Game history size check: FAILED (Expected" << games.size() << ", got" << history.size() << ")";
    } else {
        qDebug() << "  Game history size check: SUCCESS";
    }

    bool allGamesCorrect = true;
    for (int i = 0; i < qMin(history.size(), games.size()); ++i) {
        const auto &expected = games[i];
        const auto &actual = history[i];
        QString expectedMode = expected.opponent == "AI" ? "PvE" : "PvP";
        if (actual.username != expected.username ||
            actual.mode != expectedMode ||
            actual.result != expected.result ||
            actual.boardState != expected.boardState) {
            allGamesCorrect = false;
        }
    }
    qDebug() << "  Game history content check:" << (allGamesCorrect ? "SUCCESS" : "FAILED");
}
