#include "historydialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QGridLayout>

HistoryDialog::HistoryDialog(const QString &username, UserManager *userManager, QWidget *parent) : QDialog(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Game History for " + username, this); // Updated title to include username
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont font = titleLabel->font();
    font.setPointSize(20); // Increased font size
    font.setBold(true);
    titleLabel->setFont(font);
    layout->addWidget(titleLabel);

    historyList = new QListWidget(this);
    for (const auto &game : userManager->getGameHistory()) {
        if (game.username == username) { // Filter by username
            QWidget *entryWidget = new QWidget();
            QVBoxLayout *entryLayout = new QVBoxLayout(entryWidget);

            // Game details
            QLabel *detailsLabel = new QLabel(QString("Mode: %1, Result: %2\nDate: %3")
                                                  .arg(game.mode)
                                                  .arg(game.result)
                                                  .arg(game.dateTime.toString("yyyy-MM-dd HH:mm:ss")));
            detailsLabel->setStyleSheet("color: #FFFFFF; font-size: 16px; text-shadow: 1px 1px 2px #000000;");
            entryLayout->addWidget(detailsLabel);

            // Board state
            QGridLayout *boardLayout = new QGridLayout();
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    int index = i * 3 + j;
                    QString cell = game.boardState.mid(index, 1);
                    if (cell == "-") cell = "";
                    QLabel *cellLabel = new QLabel(cell);
                    cellLabel->setAlignment(Qt::AlignCenter);
                    cellLabel->setFixedSize(50, 50); // Larger cells
                    cellLabel->setStyleSheet("background: #FFFFFF; color: #1A237E; border: 2px solid #4FC3F7; border-radius: 5px; font-size: 18px; font-weight: bold;");
                    boardLayout->addWidget(cellLabel, i, j);
                }
            }
            entryLayout->addLayout(boardLayout);

            QListWidgetItem *item = new QListWidgetItem();
            item->setSizeHint(entryWidget->sizeHint());
            historyList->addItem(item);
            historyList->setItemWidget(item, entryWidget);
        }
    }
    layout->addWidget(historyList);

    QPushButton *closeButton = new QPushButton("Close", this);
    closeButton->setMinimumHeight(50); // Larger button
    layout->addWidget(closeButton);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    setStyleSheet(
        "QDialog { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #00695C, stop:1 #4DD0E1); }"
        "QLabel { color: #FFFFFF; font-size: 18px; text-shadow: 1px 1px 2px #000000; }"
        "QPushButton { background: #FF6E40; color: #1A237E; border-radius: 8px; padding: 12px; font-size: 18px; font-weight: bold; }"
        "QPushButton:hover { background: #FF8A65; transform: scale(1.05); }"
        "QListWidget { background: #FFFFFF; color: #1A237E; border: 2px solid #4FC3F7; border-radius: 8px; }"
        "QListWidget::item { background: #E1F5FE; padding: 8px; }"
        "QListWidget::item:alternate { background: #B3E5FC; }"
        "QListWidget::item:hover { background: #81D4FA; }"
        );

    setMinimumSize(600, 600); // Increased window size
}

HistoryDialog::~HistoryDialog() {}
