#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include <QDialog>
#include <QListWidget>
#include "usermanager.h"

class HistoryDialog : public QDialog {
    Q_OBJECT

public:
    explicit HistoryDialog(const QString &username, UserManager *userManager, QWidget *parent = nullptr); // Added username parameter
    ~HistoryDialog();

private:
    QListWidget *historyList;
};

#endif // HISTORYDIALOG_H
