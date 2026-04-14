#include "game.h"
#include <random>
#include <vector>
#include <algorithm>

Game::Game() {
    startNewGame();
}

void Game::startNewGame() {
    secretNumber = generateSecretNumber();
    attempts = 0;
}

QString Game::generateSecretNumber() const {
    std::vector<int> digits = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(digits.begin(), digits.end(), gen);

    if (digits[0] == 0) {
        for (int i = 1; i < 10; ++i) {
            if (digits[i] != 0) {
                std::swap(digits[0], digits[i]);
                break;
            }
        }
    }

    QString result;
    for (int i = 0; i < 4; ++i) {
        result += QString::number(digits[i]);
    }
    return result;
}

bool Game::isValidGuess(const QString &guess) const {
    if (guess.length() != 4) return false;

    for (QChar ch: guess) {
        if (!ch.isDigit()) return false;
    }

    for (int i = 0; i < guess.length(); ++i) {
        for (int j = i + 1; j < guess.length(); ++j) {
            if (guess[i] == guess[j]) return false;
        }
    }

    if (guess[0] == '0') return false;

    return true;
}

QString Game::getSecretNumber() const {
    return secretNumber;
}

QPair<int, int> Game::checkGuess(const QString &guess) {
    ++attempts;

    int bulls = 0;
    int cows = 0;

    for (int i = 0; i < 4; ++i) {
        if (guess[i] == secretNumber[i]) {
            ++bulls;
        } else if (secretNumber.contains(guess[i])) {
            ++cows;
        }
    }

    return {bulls, cows};
}

bool Game::isWin(int bulls) const {
    return bulls == 4;
}

int Game::getAttempts() const {
    return attempts;
}
