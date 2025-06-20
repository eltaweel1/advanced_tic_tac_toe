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
    font.setPointSize(20); // Increased font size
    font.setBold(true);
    titleLabel->setFont(font);
    layout->addWidget(titleLabel);

    QHBoxLayout *usernameLayout = new QHBoxLayout();
    QLabel *usernameLabel = new QLabel("Username:", this);
    usernameEdit = new QLineEdit(this);
    usernameEdit->setMinimumHeight(40); // Larger input field
    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addWidget(usernameEdit);
    layout->addLayout(usernameLayout);

    QHBoxLayout *passwordLayout = new QHBoxLayout();
    QLabel *passwordLabel = new QLabel("Password:", this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setMinimumHeight(40); // Larger input field
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(passwordEdit);
    layout->addLayout(passwordLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *loginButton = new QPushButton("Login", this);
    loginButton->setMinimumHeight(50); // Larger button
    QPushButton *registerButton = new QPushButton("Register", this);
    registerButton->setMinimumHeight(50); // Larger button
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);
    layout->addLayout(buttonLayout);

    connect(loginButton, &QPushButton::clicked, this, &LoginForm::handleLogin);
    connect(registerButton, &QPushButton::clicked, this, &LoginForm::handleRegister);

    setStyleSheet(
        "QDialog { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #00695C, stop:1 #4DD0E1); }"
        "QLabel { color: #FFFFFF; font-size: 18px; text-shadow: 1px 1px 2px #000000; }"
        "QLineEdit { background: #FFFFFF; color: #1A237E; border-radius: 8px; padding: 8px; border: 2px solid #4FC3F7; }"
        "QLineEdit:focus { border: 2px solid #FF6E40; }"
        "QPushButton { background: #FF6E40; color: #1A237E; border-radius: 8px; padding: 12px; font-size: 18px; font-weight: bold; }"
        "QPushButton:hover { background: #FF8A65; transform: scale(1.05); }"
        "QPushButton:disabled { background: #CFD8DC; color: #78909C; }"
        );

    setMinimumSize(500, 400); // Increased window size
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
