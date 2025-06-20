#include "registerform.h"
#include <QVBoxLayout>
#include <QMessageBox>

RegisterForm::RegisterForm(UserManager *userManager, QWidget *parent) : QWidget(parent), userManager(userManager) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Register", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont font = titleLabel->font();
    font.setPointSize(20); // Increased font size
    font.setBold(true);
    titleLabel->setFont(font);
    layout->addWidget(titleLabel);

    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Choose a username");
    usernameEdit->setMinimumHeight(40); // Larger input field
    layout->addWidget(usernameEdit);

    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Choose a password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setMinimumHeight(40); // Larger input field
    layout->addWidget(passwordEdit);

    registerButton = new QPushButton("Register", this);
    registerButton->setMinimumHeight(50); // Larger button
    backButton = new QPushButton("Back to Login", this);
    backButton->setMinimumHeight(50); // Larger button
    layout->addWidget(registerButton);
    layout->addWidget(backButton);

    connect(registerButton, &QPushButton::clicked, this, &RegisterForm::handleRegister);
    connect(backButton, &QPushButton::clicked, this, &RegisterForm::handleBack);

    setStyleSheet(
        "QWidget { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #00695C, stop:1 #4DD0E1); }"
        "QLabel { color: #FFFFFF; font-size: 18px; text-shadow: 1px 1px 2px #000000; }"
        "QLineEdit { background: #FFFFFF; border-radius: 8px; padding: 8px; color: #1A237E; border: 2px solid #4FC3F7; }"
        "QLineEdit:focus { border: 2px solid #FF6E40; }"
        "QPushButton { background: #FF6E40; color: #1A237E; border-radius: 8px; padding: 12px; font-size: 18px; font-weight: bold; }"
        "QPushButton:hover { background: #FF8A65; transform: scale(1.05); }"
        "QPushButton:disabled { background: #CFD8DC; color: #78909C; }"
        );

    setMinimumSize(500, 400); // Increased window size
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
