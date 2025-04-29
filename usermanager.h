#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QString>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QDateTime>  // Added for QDateTime

struct Game {
    QString username;
    QString mode;
    QString result;
    QString boardState;  // Added to store the final board state
    QDateTime dateTime;  // Added to store the date and time of the game
};

class UserManager {
public:
    UserManager();
    bool registerUser(const QString &username, const QString &password);
    bool loginUser(const QString &username, const QString &password);
    void addGameHistory(const QString &username, const QString &mode, const QString &result, const QString &boardState, const QDateTime &dateTime);
    QList<Game> getGameHistory() const;

private:
    struct User {
        QString username;
        QString password;
    };
    QList<User> users;
    QList<Game> gameHistory;
};

#endif // USERMANAGER_H
