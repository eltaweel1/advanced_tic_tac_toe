#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include "usermanager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(const QString &username, UserManager *userManager, QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void newGameRequested(const QString &username);
    void viewHistoryRequested();

private slots:
    void handleNewGame();
    void handleViewHistory();
    void handleExit();

private:
    QString username;
    UserManager *userManager;
    QLabel *titleLabel;
};

#endif // MAINWINDOW_H
