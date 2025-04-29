#include "usermanager.h"

UserManager::UserManager() {
    QFile file("users.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() == 2) {
                users.append({parts[0], parts[1]});
            }
        }
        file.close();
    }

    QFile historyFile("history.txt");
    if (historyFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&historyFile);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() == 5) {
                Game game;
                game.username = parts[0];
                game.mode = parts[1];
                game.result = parts[2];
                game.boardState = parts[3];
                game.dateTime = QDateTime::fromString(parts[4], "yyyy-MM-dd HH:mm:ss");
                gameHistory.append(game);
            }
        }
        historyFile.close();
    }
}

bool UserManager::registerUser(const QString &username, const QString &password) {
    for (const auto &user : users) {
        if (user.username == username) return false;
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
