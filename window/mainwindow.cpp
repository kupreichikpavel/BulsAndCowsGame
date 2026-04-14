#include "mainwindow.h"
#include "../nameDialog/namedialog.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QFile>
#include <QTextStream>
#include <QRegularExpressionValidator>
#include <QAbstractItemView>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUi();
    loadRecords();
    startNewGame();
}

void MainWindow::setupUi() {
    setWindowTitle("Быки и коровы");
    resize(650, 500);

    auto *central = new QWidget(this);
    setCentralWidget(central);

    auto *mainLayout = new QVBoxLayout(central);

    auto *topButtonsLayout = new QHBoxLayout();

    newGameButton = new QPushButton("Новая игра", this);
    recordsButton = new QPushButton("Рекорды", this);
    showAnswerButton = new QPushButton("Показать ответ", this);
    clearRecordsButton = new QPushButton("Очистить рекорды", this);

    topButtonsLayout->addWidget(newGameButton);
    topButtonsLayout->addWidget(recordsButton);
    topButtonsLayout->addWidget(showAnswerButton);
    topButtonsLayout->addWidget(clearRecordsButton);

    mainLayout->addLayout(topButtonsLayout);

    statusLabel = new QLabel("Добро пожаловать в игру", this);
    mainLayout->addWidget(statusLabel);

    auto *inputLayout = new QHBoxLayout();

    inputEdit = new QLineEdit(this);
    inputEdit->setPlaceholderText("Введите 4-значное число без повторяющихся цифр");
    inputEdit->setMaxLength(4);
    inputEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d{0,4}"), this));

    checkButton = new QPushButton("Проверить", this);

    inputLayout->addWidget(inputEdit);
    inputLayout->addWidget(checkButton);

    mainLayout->addLayout(inputLayout);

    attemptsTable = new QTableWidget(this);
    attemptsTable->setColumnCount(2);
    attemptsTable->setHorizontalHeaderLabels({"Число", "Результат"});
    attemptsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    attemptsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mainLayout->addWidget(attemptsTable);

    connect(newGameButton, &QPushButton::clicked, this, &MainWindow::startNewGame);
    connect(checkButton, &QPushButton::clicked, this, &MainWindow::checkInput);
    connect(recordsButton, &QPushButton::clicked, this, &MainWindow::showRecords);
    connect(showAnswerButton, &QPushButton::clicked, this, &MainWindow::showAnswer);
    connect(clearRecordsButton, &QPushButton::clicked, this, &MainWindow::clearRecords);
    connect(inputEdit, &QLineEdit::returnPressed, this, &MainWindow::checkInput);
}

void MainWindow::startNewGame() {
    game.startNewGame();
    attemptsTable->setRowCount(0);
    inputEdit->clear();
    inputEdit->setFocus();
    statusLabel->setText("Новая игра началась");
}

void MainWindow::checkInput() {
    const QString guess = inputEdit->text().trimmed();

    if (!game.isValidGuess(guess)) {
        QMessageBox::warning(
            this,
            "Ошибка",
            "Введите корректное 4-значное число без повторяющихся цифр и не начинающееся с 0."
        );
        return;
    }

    auto result = game.checkGuess(guess);
    addAttemptToTable(guess, result.first, result.second);

    if (game.isWin(result.first)) {
        statusLabel->setText("Вы угадали число");
        handleWin();
    } else {
        statusLabel->setText(
            QString("Попытка %1: быков %2, коров %3")
                .arg(game.getAttempts())
                .arg(result.first)
                .arg(result.second)
        );
    }

    inputEdit->clear();
    inputEdit->setFocus();
}

void MainWindow::showRecords() {
    RecordsDialog dialog(records, this);
    dialog.exec();
}

void MainWindow::showAnswer() {
    QMessageBox::information(this, "Секретное число", "Ответ: " + game.getSecretNumber());
}

void MainWindow::clearRecords() {
    auto reply = QMessageBox::question(
        this,
        "Очистка",
        "Удалить все рекорды?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply != QMessageBox::Yes) return;

    records.clear();
    saveRecords();

    QMessageBox::information(this, "Готово", "Рекорды очищены");
}

void MainWindow::addAttemptToTable(const QString& guess, int bulls, int cows) {
    int row = attemptsTable->rowCount();
    attemptsTable->insertRow(row);

    attemptsTable->setItem(row, 0, new QTableWidgetItem(guess));
    attemptsTable->setItem(row, 1, new QTableWidgetItem(
        QString("Быков: %1, Коров: %2").arg(bulls).arg(cows)
    ));
}

void MainWindow::loadRecords() {
    records.clear();

    QFile file("records.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(';');
        if (parts.size() != 2) continue;

        bool ok = false;
        int attempts = parts[0].toInt(&ok);
        if (!ok) continue;

        records.push_back({attempts, parts[1]});
    }

    sortAndTrimRecords();
}

void MainWindow::saveRecords() {
    QFile file("records.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл");
        return;
    }

    QTextStream out(&file);
    for (const auto& record : records) {
        out << record.attempts << ";" << record.name << "\n";
    }
}

void MainWindow::sortAndTrimRecords() {
    std::sort(records.begin(), records.end(), [](const Record& a, const Record& b) {
        return a.attempts < b.attempts;
    });

    while (records.size() > 10) {
        records.removeLast();
    }
}

bool MainWindow::isHighScore(int attempts) const {
    if (records.size() < 10) return true;
    return attempts < records.last().attempts;
}

void MainWindow::handleWin() {
    int attempts = game.getAttempts();

    QMessageBox::information(
        this,
        "Победа",
        QString("Вы угадали число за %1 попыток").arg(attempts)
    );

    if (!isHighScore(attempts)) return;

    NameDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.getName();
        if (name.isEmpty()) name = "Без имени";

        records.push_back({attempts, name});
        sortAndTrimRecords();
        saveRecords();
    }
}