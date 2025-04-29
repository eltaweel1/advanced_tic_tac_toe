#include "historydialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QGridLayout>

HistoryDialog::HistoryDialog(UserManager *userManager, QWidget *parent) : QDialog(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Game History", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont font = titleLabel->font();
    font.setPointSize(16);
    font.setBold(true);
    titleLabel->setFont(font);
    layout->addWidget(titleLabel);

    historyList = new QListWidget(this);
    for (const auto &game : userManager->getGameHistory()) {
        QWidget *entryWidget = new QWidget();
        QVBoxLayout *entryLayout = new QVBoxLayout(entryWidget);

        // Game details
        QLabel *detailsLabel = new QLabel(QString("%1 - Mode: %2, Result: %3\nDate: %4")
                                              .arg(game.username)
                                              .arg(game.mode)
                                              .arg(game.result)
                                              .arg(game.dateTime.toString("yyyy-MM-dd HH:mm:ss")));
        detailsLabel->setStyleSheet("color: #FFFFFF; font-size: 12px;");
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
                cellLabel->setFixedSize(30, 30);
                cellLabel->setStyleSheet("background: #FFFFFF; color: #212121; border: 1px solid #90CAF9; font-size: 14px;");
                boardLayout->addWidget(cellLabel, i, j);
            }
        }
        entryLayout->addLayout(boardLayout);

        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(entryWidget->sizeHint());
        historyList->addItem(item);
        historyList->setItemWidget(item, entryWidget);
    }
    layout->addWidget(historyList);

    QPushButton *closeButton = new QPushButton("Close", this);
    layout->addWidget(closeButton);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    setStyleSheet(
        "QDialog { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1565C0, stop:1 #42A5F5); }"
        "QLabel { color: #FFFFFF; font-size: 14px; }"
        "QPushButton { background: #42A5F5; color: #212121; border-radius: 5px; padding: 8px; font-size: 14px; }"
        "QPushButton:hover { background: #64B5F6; }"
        "QListWidget { background: #FFFFFF; color: #212121; border: 1px solid #90CAF9; }"
        "QListWidget::item:hover { background: #BBDEFB; }"
        );
}

HistoryDialog::~HistoryDialog() {}
