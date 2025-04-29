#ifndef REGISTERFORM_H
#define REGISTERFORM_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "usermanager.h"

class RegisterForm : public QWidget {
    Q_OBJECT

public:
    explicit RegisterForm(UserManager *userManager, QWidget *parent = nullptr);
    ~RegisterForm();

signals:
    void registrationCompleted();
    void backToLoginRequested();

private slots:
    void handleRegister();
    void handleBack();

private:
    UserManager *userManager;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *registerButton;
    QPushButton *backButton;
};

#endif // REGISTERFORM_H
