#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>
#include "usermanager.h"

class QLineEdit;

class LoginForm : public QDialog {
    Q_OBJECT

public:
    LoginForm(UserManager *userManager, QWidget *parent = nullptr);
    ~LoginForm();

signals:
    void loginRequested(const QString &username);
    void registerRequested();

private slots:
    void handleLogin();
    void handleRegister();

private:
    UserManager *userManager;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
};

#endif // LOGINFORM_H
