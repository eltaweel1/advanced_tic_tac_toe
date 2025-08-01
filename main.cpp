#include <QApplication>
#include "loginform.h"
#include "registerform.h"
#include "mainwindow.h"
#include "gamesetupdialog.h"
#include "board.h"
#include "historydialog.h"
#include "usermanager.h"

void showSetupDialog(const QString &username, UserManager *userManager, MainWindow *mainWindow);
void showLoginForm(UserManager *userManager);

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    UserManager *userManager = new UserManager();
    showLoginForm(userManager); // Start with login form

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
                             board->deleteLater();
                             mainWindow->show();
                             mainWindow->raise();
                             mainWindow->activateWindow();
                         });
                     });

    QObject::connect(setupDialog, &GameSetupDialog::backToMainRequested, [setupDialog, mainWindow]() {
        setupDialog->close();
        setupDialog->deleteLater();
        mainWindow->show();
        mainWindow->raise();
        mainWindow->activateWindow();
    });

    setupDialog->exec();
}

void showLoginForm(UserManager *userManager) {
    LoginForm *loginForm = new LoginForm(userManager);
    loginForm->show();

    QObject::connect(loginForm, &LoginForm::loginRequested, [loginForm, userManager](const QString &username) {
        loginForm->hide();
        loginForm->close();
        loginForm->deleteLater();

        MainWindow *mainWindow = new MainWindow(username, userManager);
        mainWindow->show();
        mainWindow->raise();
        mainWindow->activateWindow();

        QObject::connect(mainWindow, &MainWindow::newGameRequested, [userManager, mainWindow](const QString &username) {
            showSetupDialog(username, userManager, mainWindow);
        });

        QObject::connect(mainWindow, &MainWindow::viewHistoryRequested, [userManager](const QString &username) {
            HistoryDialog *historyDialog = new HistoryDialog(username, userManager);
            historyDialog->exec();
        });

        QObject::connect(mainWindow, &MainWindow::logoutRequested, [mainWindow, userManager]() {
            mainWindow->hide();
            mainWindow->close();
            mainWindow->deleteLater();
            showLoginForm(userManager);
        });
    });

    QObject::connect(loginForm, &LoginForm::registerRequested, [loginForm, userManager]() {
        loginForm->hide();
        RegisterForm *registerForm = new RegisterForm(userManager); // Create new RegisterForm
        registerForm->show();

        QObject::connect(registerForm, &RegisterForm::registrationCompleted, [registerForm, loginForm]() {
            registerForm->hide();
            registerForm->close();
            registerForm->deleteLater();
            loginForm->show();
        });

        QObject::connect(registerForm, &RegisterForm::backToLoginRequested, [registerForm, loginForm]() {
            registerForm->hide();
            registerForm->close();
            registerForm->deleteLater();
            loginForm->show();
        });
    });
}
//---------------------------------------------------------------------------------------------------------------------------
// GameLogic and Ai test
// #include <QCoreApplication>
// #include "gamelogictest.h"
// #include "aitest.h"

// int main(int argc, char *argv[])
// {
//     QCoreApplication a(argc, argv);

//     GameLogicTest::runTests();
//     AITest::runTests();

//     return 0; // Not using a.exec() since it's a console-based test
// }




//-------------------------------------------------------------------------------------------------------------------------------------
//DataBase Test

// #include <QApplication>
// #include "usermanager.h"
// #include "DatabaseTest.h"



// int main(int argc, char *argv[]) {
//     QApplication app(argc, argv);
//     UserManager *userManager = new UserManager();
//     DatabaseTest *dbTest = new DatabaseTest(userManager);
//     dbTest->runAllTests();
//     delete dbTest;
//     delete userManager;
//     return 0;
// }

//-------------------------------------------------------------------------------------------------------------------------------------

// #include "IntegrationTest.h"

// int main() {
//     IntegrationTest::run();
//     return 0;
// }
//----------------------------------------------------------------------------------------------------------------------------------------------
//all tests
// #include <QCoreApplication>
// #include <QApplication>

// #include "gamelogictest.h"
// #include "aitest.h"
// #include "usermanager.h"
// #include "databasetest.h"
// #include "integrationtest.h"

// int main(int argc, char *argv[]) {
//     // Step 1: Run console-based tests (GameLogic and AI)
//     {
//         QCoreApplication coreApp(argc, argv);  // QCoreApplication for non-GUI tests
//         GameLogicTest::runTests();
//         AITest::runTests();
//     }

//     // Step 2: Run GUI-related Database test
//     {
//         QApplication guiApp(argc, argv);  // QApplication for GUI components (e.g., Database)
//         UserManager *userManager = new UserManager();
//         DatabaseTest *dbTest = new DatabaseTest(userManager);
//         dbTest->runAllTests();
//         delete dbTest;
//         delete userManager;
//     }

//     // Step 3: Run Integration tests
//     IntegrationTest::run();

//     return 0;
// }

