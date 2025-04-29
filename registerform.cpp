#include "registerform.h"
#include <QVBoxLayout>
#include <QMessageBox>

RegisterForm::RegisterForm(UserManager *userManager, QWidget *parent) : QWidget(parent), userManager(userManager) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Register", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont font = titleLabel->font();
    font.setPointSize(18);
    font.setBold(true);
    titleLabel->setFont(font);
    layout->addWidget(titleLabel);

    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Choose a username");
    layout->addWidget(usernameEdit);

    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Choose a password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordEdit);

    registerButton = new QPushButton("Register", this);
    backButton = new QPushButton("Back to Login", this);
    layout->addWidget(registerButton);
    layout->addWidget(backButton);

    connect(registerButton, &QPushButton::clicked, this, &RegisterForm::handleRegister);
    connect(backButton, &QPushButton::clicked, this, &RegisterForm::handleBack);

    setStyleSheet(
        "QWidget { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1565C0, stop:1 #42A5F5); }"
        "QLabel { color: #FFFFFF; font-size: 16px; }" // White text for labels
        "QLineEdit { background: #FFFFFF; border-radius: 5px; padding: 5px; color: #212121; border: 1px solid #90CAF9; }" // Dark text for input fields
        "QPushButton { background: #42A5F5; color: #212121; border-radius: 5px; padding: 8px; font-size: 14px; }" // Dark text for buttons
        "QPushButton:hover { background: #64B5F6; }"
        "QPushButton:disabled { background: #B0BEC5; }"
        );
}

RegisterForm::~RegisterForm() {}

void RegisterForm::handleRegister() {
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Registration Failed", "Username and password cannot be empty.");
        return;
    }
    if (userManager->registerUser(username, password)) {
        QMessageBox::information(this, "Success", "Registration successful! Please login.");
        emit registrationCompleted();
    } else {
        QMessageBox::warning(this, "Registration Failed", "Username already exists.");
    }
}

void RegisterForm::handleBack() {
    emit backToLoginRequested();
}
