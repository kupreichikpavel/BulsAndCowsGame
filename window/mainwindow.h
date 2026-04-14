#pragma once

#include <QMainWindow>
#include <QVector>
#include "../game/game.h"
#include "../recordDialog/recordsdialog.h"

class QLineEdit;
class QPushButton;
class QTableWidget;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    Game game;
    QVector<Record> records;

    QLineEdit* inputEdit;
    QPushButton* checkButton;
    QPushButton* newGameButton;
    QPushButton* recordsButton;
    QPushButton* showAnswerButton;
    QPushButton* clearRecordsButton;
    QTableWidget* attemptsTable;
    QLabel* statusLabel;

    void setupUi();
    void startNewGame();
    void checkInput();
    void showRecords();
    void showAnswer();
    void clearRecords();
    void addAttemptToTable(const QString& guess, int bulls, int cows);
    void loadRecords();
    void saveRecords();
    void sortAndTrimRecords();
    bool isHighScore(int attempts) const;
    void handleWin();
};