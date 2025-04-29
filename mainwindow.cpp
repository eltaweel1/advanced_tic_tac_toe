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
    font.setPointSize(18);
    font.setBold(true);
    titleLabel->setFont(font);
    layout->addWidget(titleLabel);

    QPushButton *newGameButton = new QPushButton("New Game", this);
    QPushButton *historyButton = new QPushButton("View History", this);
    QPushButton *exitButton = new QPushButton("Exit", this);

    layout->addWidget(newGameButton);
    layout->addWidget(historyButton);
    layout->addWidget(exitButton);

    connect(newGameButton, &QPushButton::clicked, this, &MainWindow::handleNewGame);
    connect(historyButton, &QPushButton::clicked, this, &MainWindow::handleViewHistory);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::handleExit);

    setWindowTitle("Tic Tac Toe");
    setStyleSheet(
        "QMainWindow { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1565C0, stop:1 #42A5F5); }"
        "QLabel { color: #FFFFFF; font-size: 16px; }" // White text for labels
        "QPushButton { background: #42A5F5; color: #212121; border-radius: 5px; padding: 8px; font-size: 14px; }" // Dark text for buttons
        "QPushButton:hover { background: #64B5F6; }"
        "QPushButton:disabled { background: #B0BEC5; }"
        );
}

MainWindow::~MainWindow() {}

void MainWindow::handleNewGame() {
    emit newGameRequested(username);
}

void MainWindow::handleViewHistory() {
    emit viewHistoryRequested();
}

void MainWindow::handleExit() {
    QApplication::quit();
}
