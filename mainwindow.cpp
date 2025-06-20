#include "mainwindow.h"
#include <QVBoxLayout>
#include <QApplication>

MainWindow::MainWindow(const QString &username, UserManager *userManager, QWidget *parent)
    : QMainWindow(parent), username(username), userManager(userManager) {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    titleLabel = new QLabel("Tic Tac Toe\nMain Menu", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont font = titleLabel->font();
    font.setPointSize(22);
    font.setBold(true);
    titleLabel->setFont(font);
    layout->addWidget(titleLabel);

    QPushButton *newGameButton = new QPushButton("New Game", this);
    newGameButton->setMinimumHeight(60);
    QPushButton *historyButton = new QPushButton("View History", this);
    historyButton->setMinimumHeight(60);
    QPushButton *logoutButton = new QPushButton("Logout", this); // Moved up
    logoutButton->setMinimumHeight(60);
    QPushButton *exitButton = new QPushButton("Exit", this); // Moved down
    exitButton->setMinimumHeight(60);

    layout->addWidget(newGameButton);
    layout->addWidget(historyButton);
    layout->addWidget(logoutButton); // Add Logout before Exit
    layout->addWidget(exitButton); // Add Exit last

    connect(newGameButton, &QPushButton::clicked, this, &MainWindow::handleNewGame);
    connect(historyButton, &QPushButton::clicked, this, &MainWindow::handleViewHistory);
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::handleLogout);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::handleExit);

    setWindowTitle("Tic Tac Toe");
    setStyleSheet(
        "QMainWindow { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #00695C, stop:1 #4DD0E1); }"
        "QLabel { color: #FFFFFF; font-size: 20px; text-shadow: 1px 1px 2px #000000; }"
        "QPushButton { background: #FF6E40; color: #1A237E; border-radius: 8px; padding: 12px; font-size: 18px; font-weight: bold; }"
        "QPushButton:hover { background: #FF8A65; transform: scale(1.05); }"
        "QPushButton:disabled { background: #CFD8DC; color: #78909C; }"
        );

    resize(600, 500);
}

MainWindow::~MainWindow() {}

void MainWindow::handleNewGame() {
    emit newGameRequested(username);
}

void MainWindow::handleViewHistory() {
    emit viewHistoryRequested(username);
}

void MainWindow::handleExit() {
    QApplication::quit();
}

void MainWindow::handleLogout() {
    emit logoutRequested();
}
