#include <QApplication>
#include "loginform.h"
#include "registerform.h"
#include "mainwindow.h"
#include "gamesetupdialog.h"
#include "board.h"
#include "historydialog.h"
#include "usermanager.h"

void showSetupDialog(const QString &username, UserManager *userManager, MainWindow *mainWindow);

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    UserManager *userManager = new UserManager();
    LoginForm *loginForm = new LoginForm(userManager);
    RegisterForm *registerForm = new RegisterForm(userManager);
    MainWindow *mainWindow = nullptr;

    loginForm->show();

    QObject::connect(loginForm, &LoginForm::loginRequested, [&mainWindow, loginForm, userManager](const QString &username) {
        loginForm->hide();
        loginForm->close();
        loginForm->deleteLater();
        mainWindow = new MainWindow(username, userManager);
        mainWindow->show();
        mainWindow->raise();
        mainWindow->activateWindow();

        QObject::connect(mainWindow, &MainWindow::newGameRequested, [userManager, mainWindow](const QString &username) {
            showSetupDialog(username, userManager, mainWindow);
        });

        QObject::connect(mainWindow, &MainWindow::viewHistoryRequested, [userManager]() {
            HistoryDialog *historyDialog = new HistoryDialog(userManager);
            historyDialog->exec();
        });
    });

    QObject::connect(loginForm, &LoginForm::registerRequested, [loginForm, registerForm]() {
        loginForm->hide();
        registerForm->show();
    });

    QObject::connect(registerForm, &RegisterForm::registrationCompleted, [registerForm, loginForm]() {
        registerForm->hide();
        loginForm->show();
    });

    QObject::connect(registerForm, &RegisterForm::backToLoginRequested, [registerForm, loginForm]() {
        registerForm->hide();
        loginForm->show();
    });

    return app.exec();
}

void showSetupDialog(const QString &username, UserManager *userManager, MainWindow *mainWindow) {
    GameSetupDialog *setupDialog = new GameSetupDialog(username, userManager, mainWindow);

    QObject::connect(setupDialog, &GameSetupDialog::setupCompleted,
                     [mainWindow](const QString &username, char playerSymbol, QString mode, QString aiDifficulty, UserManager *userManager) {
                         mainWindow->hide();
                         Board *board = new Board(username, playerSymbol, mode, aiDifficulty, userManager);
                         board->show();
                         board->raise();
                         board->activateWindow();
                         QObject::connect(board, &Board::returnToMainRequested, [board, mainWindow]() {
                             board->close();
                             mainWindow->show();
                             mainWindow->raise();
                             mainWindow->activateWindow();
                         });
                     });

    QObject::connect(setupDialog, &GameSetupDialog::backToMainRequested, [setupDialog, mainWindow]() {
        setupDialog->close();
        mainWindow->show();
        mainWindow->raise();
        mainWindow->activateWindow();
    });

    setupDialog->exec();
}
