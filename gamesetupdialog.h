#ifndef GAMESETUPDIALOG_H
#define GAMESETUPDIALOG_H

#include <QDialog>
#include "usermanager.h"

class QRadioButton;
class QComboBox;

class GameSetupDialog : public QDialog {
    Q_OBJECT

public:
    GameSetupDialog(const QString &username, UserManager *userManager, QWidget *parent = nullptr);
    ~GameSetupDialog();

signals:
    void setupCompleted(const QString &username, char playerSymbol, QString mode, QString aiDifficulty, UserManager *userManager);
    void backToMainRequested();

private slots:
    void handleStart();
    void handleBack();

private:
    QString username;
    UserManager *userManager;
    QRadioButton *pvpRadio;
    QRadioButton *pveRadio;
    QRadioButton *xRadio;
    QRadioButton *oRadio;
    QComboBox *difficultyCombo;
};

#endif // GAMESETUPDIALOG_H
