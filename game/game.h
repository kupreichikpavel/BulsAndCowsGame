#pragma once

#include <QString>
#include <QPair>

class Game {
public:
    Game();

    void startNewGame();

    bool isValidGuess(const QString &guess) const;

    QPair<int, int> checkGuess(const QString &guess);

    bool isWin(int bulls) const;

    int getAttempts() const;
    
    QString getSecretNumber() const;

private:
    QString secretNumber;
    int attempts;

    QString generateSecretNumber() const;
};
