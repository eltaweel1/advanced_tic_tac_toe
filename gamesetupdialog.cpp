#include "gamesetupdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QComboBox>
#include <QPushButton>
#include <QButtonGroup>

GameSetupDialog::GameSetupDialog(const QString &username, UserManager *userManager, QWidget *parent)
    : QDialog(parent), username(username), userManager(userManager) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Title
    QLabel *titleLabel = new QLabel("Game Setup", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont font = titleLabel->font();
    font.setPointSize(16);
    font.setBold(true);
    titleLabel->setFont(font);
    layout->addWidget(titleLabel);

    // Game Mode Selection
    QHBoxLayout *modeLayout = new QHBoxLayout();
    QLabel *modeLabel = new QLabel("Game Mode:", this);
    pvpRadio = new QRadioButton("Player vs Player", this);
    pveRadio = new QRadioButton("Player vs AI", this);

    // Group the radio buttons to ensure only one can be selected at a time
    QButtonGroup *modeGroup = new QButtonGroup(this);
    modeGroup->addButton(pvpRadio);
    modeGroup->addButton(pveRadio);

    pvpRadio->setChecked(true); // Default to PvP
    modeLayout->addWidget(modeLabel);
    modeLayout->addWidget(pvpRadio);
    modeLayout->addWidget(pveRadio);
    layout->addLayout(modeLayout);

    // Symbol Selection
    QHBoxLayout *symbolLayout = new QHBoxLayout();
    QLabel *symbolLabel = new QLabel("Play as:", this);
    xRadio = new QRadioButton("X", this);
    oRadio = new QRadioButton("O", this);

    // Group the symbol radio buttons
    QButtonGroup *symbolGroup = new QButtonGroup(this);
    symbolGroup->addButton(xRadio);
    symbolGroup->addButton(oRadio);

    xRadio->setChecked(true); // Default to X
    symbolLayout->addWidget(symbolLabel);
    symbolLayout->addWidget(xRadio);
    symbolLayout->addWidget(oRadio);
    layout->addLayout(symbolLayout);

    // AI Difficulty Selection
    QHBoxLayout *difficultyLayout = new QHBoxLayout();
    QLabel *difficultyLabel = new QLabel("AI Difficulty:", this);
    difficultyCombo = new QComboBox(this);
    difficultyCombo->addItems({"Easy", "Medium", "Hard"});
    difficultyLayout->addWidget(difficultyLabel);
    difficultyLayout->addWidget(difficultyCombo);
    layout->addLayout(difficultyLayout);
    difficultyCombo->setEnabled(false); // Disabled by default (enabled only in PvE mode)

    // Enable/disable difficulty combo based on mode selection
    connect(pveRadio, &QRadioButton::toggled, difficultyCombo, &QComboBox::setEnabled);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *startButton = new QPushButton("Start Game", this);
    QPushButton *backButton = new QPushButton("Back to Main Menu", this);
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(backButton);
    layout->addLayout(buttonLayout);

    connect(startButton, &QPushButton::clicked, this, &GameSetupDialog::handleStart);
    connect(backButton, &QPushButton::clicked, this, &GameSetupDialog::handleBack);

    // Styling
    setStyleSheet(
        "QDialog { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1565C0, stop:1 #42A5F5); }"
        "QLabel { color: #FFFFFF; font-size: 14px; }"
        "QRadioButton { color: #FFFFFF; }"
        "QComboBox { background: #FFFFFF; color: #212121; border-radius: 5px; padding: 5px; }"
        "QComboBox::drop-down { border: none; }"
        "QComboBox::down-arrow { image: url(:/icons/down-arrow.png); }"
        "QPushButton { background: #42A5F5; color: #212121; border-radius: 5px; padding: 8px; font-size: 14px; }"
        "QPushButton:hover { background: #64B5F6; }"
        "QPushButton:disabled { background: #B0BEC5; }"
        );
}

GameSetupDialog::~GameSetupDialog() {}

void GameSetupDialog::handleStart() {
    // Determine the selected mode
    QString mode = pvpRadio->isChecked() ? "PvP" : "PvE";
    char playerSymbol = xRadio->isChecked() ? 'X' : 'O';
    QString aiDifficulty = difficultyCombo->currentText();
    emit setupCompleted(username, playerSymbol, mode, aiDifficulty, userManager);
    accept();
}

void GameSetupDialog::handleBack() {
    emit backToMainRequested();
    reject();
}
