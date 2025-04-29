#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include <QDialog>
#include <QListWidget>
#include "usermanager.h"

class HistoryDialog : public QDialog {
    Q_OBJECT

public:
    explicit HistoryDialog(UserManager *userManager, QWidget *parent = nullptr);
    ~HistoryDialog();

private:
    QListWidget *historyList;
};

#endif // HISTORYDIALOG_H
