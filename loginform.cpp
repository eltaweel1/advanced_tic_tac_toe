#include "loginform.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

LoginForm::LoginForm(UserManager *userManager, QWidget *parent) : QDialog(parent), userManager(userManager) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Login", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont font = titleLabel->font();
    font.setPointSize(16);
    font.setBold(true);
    titleLabel->setFont(font);
    layout->addWidget(titleLabel);

    QHBoxLayout *usernameLayout = new QHBoxLayout();
    QLabel *usernameLabel = new QLabel("Username:", this);
    usernameEdit = new QLineEdit(this);
    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addWidget(usernameEdit);
    layout->addLayout(usernameLayout);

    QHBoxLayout *passwordLayout = new QHBoxLayout();
    QLabel *passwordLabel = new QLabel("Password:", this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(passwordEdit);
    layout->addLayout(passwordLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *loginButton = new QPushButton("Login", this);
    QPushButton *registerButton = new QPushButton("Register", this);
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);
    layout->addLayout(buttonLayout);

    connect(loginButton, &QPushButton::clicked, this, &LoginForm::handleLogin);
    connect(registerButton, &QPushButton::clicked, this, &LoginForm::handleRegister);

    setStyleSheet(
        "QDialog { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1565C0, stop:1 #42A5F5); }"
        "QLabel { color: #FFFFFF; font-size: 14px; }"
        "QLineEdit { background: #FFFFFF; color: #212121; border-radius: 5px; padding: 5px; }"
        "QPushButton { background: #42A5F5; color: #212121; border-radius: 5px; padding: 8px; font-size: 14px; }"
        "QPushButton:hover { background: #64B5F6; }"
        );
}

LoginForm::~LoginForm() {}

void LoginForm::handleLogin() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    if (userManager->loginUser(username, password)) {
        emit loginRequested(username);
        accept();
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
    }
}

void LoginForm::handleRegister() {
    emit registerRequested();
}
