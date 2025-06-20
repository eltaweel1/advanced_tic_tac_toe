#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QString>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

struct Game {
    QString username;
    QString mode;
    QString result;
    QString boardState;
    QDateTime dateTime;
};

class UserManager {
public:
    UserManager();
    bool registerUser(const QString &username, const QString &password);
    bool loginUser(const QString &username, const QString &password);
    bool validateUser(const QString &username, const QString &password);
    bool deleteUser(const QString &username);
    void addGameHistory(const QString &username, const QString &mode, const QString &result, const QString &boardState, const QDateTime &dateTime);
    QList<Game> getGameHistory() const;
    void clearUsers();

private:
    struct User {
        QString username;
        QString password;
    };
    QList<User> users;
    QList<Game> gameHistory;
    void saveUsersToFile();
};

#endif // USERMANAGER_H
