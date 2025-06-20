#include "usermanager.h"

UserManager::UserManager() {
    users.clear();
    gameHistory.clear();
    QFile userFile("users.txt");
    if (userFile.exists()) {
        userFile.remove();
    }
    QFile historyFile("history.txt");
    if (historyFile.exists()) {
        historyFile.remove();
    }
}

bool UserManager::registerUser(const QString &username, const QString &password) {
    for (const auto &user : users) {
        if (user.username == username) {
            return false;
        }
    }
    users.append({username, password});
    QFile file("users.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << username << "," << password << "\n";
        file.close();
        return true;
    }
    return false;
}

bool UserManager::loginUser(const QString &username, const QString &password) {
    for (const auto &user : users) {
        if (user.username == username && user.password == password) return true;
    }
    return false;
}

bool UserManager::validateUser(const QString &username, const QString &password) {
    return loginUser(username, password);
}

bool UserManager::deleteUser(const QString &username) {
    int index = -1;
    for (int i = 0; i < users.size(); ++i) {
        if (users[i].username == username) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return false;
    }
    users.removeAt(index);
    saveUsersToFile();
    return true;
}

void UserManager::saveUsersToFile() {
    QFile file("users.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const auto &user : users) {
            out << user.username << "," << user.password << "\n";
        }
        file.close();
    }
}

void UserManager::addGameHistory(const QString &username, const QString &mode, const QString &result, const QString &boardState, const QDateTime &dateTime) {
    gameHistory.append({username, mode, result, boardState, dateTime});
    QFile file("history.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << username << "," << mode << "," << result << "," << boardState << "," << dateTime.toString("yyyy-MM-dd HH:mm:ss") << "\n";
        file.close();
    }
}

QList<Game> UserManager::getGameHistory() const {
    return gameHistory;
}

void UserManager::clearUsers() {
    users.clear();
}
