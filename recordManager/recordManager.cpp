#include "recordManager.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <algorithm>

RecordManager::RecordManager(const QString& fileName)
    : fileName(fileName) {
    loadFromFile();
}

void RecordManager::loadFromFile() {
    records.clear();

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();
        if (line.isEmpty()) {
            continue;
        }

        const QStringList parts = line.split(';');
        if (parts.size() != 2) {
            continue;
        }

        bool ok = false;
        const int attempts = parts[0].toInt(&ok);
        if (!ok) {
            continue;
        }

        Record record;
        record.attempts = attempts;
        record.name = parts[1].trimmed();

        records.push_back(record);
    }

    sortRecords();
    trimToTop10();
}

void RecordManager::saveToFile() const {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream out(&file);

    for (const auto& record : records) {
        out << record.attempts << ";" << record.name << "\n";
    }
}

void RecordManager::addRecord(const QString& name, int attempts) {
    Record record;
    record.name = name.trimmed().isEmpty() ? "Без имени" : name.trimmed();
    record.attempts = attempts;

    records.push_back(record);

    sortRecords();
    trimToTop10();
    saveToFile();
}

bool RecordManager::isHighScore(int attempts) const {
    if (records.size() < 10) {
        return true;
    }

    return attempts < records.back().attempts;
}

QVector<Record> RecordManager::getTopRecords() const {
    return records;
}

void RecordManager::clearRecords() {
    records.clear();
    saveToFile();
}

void RecordManager::sortRecords() {
    std::sort(records.begin(), records.end(),
              [](const Record& a, const Record& b) {
                  return a.attempts < b.attempts;
              });
}

void RecordManager::trimToTop10() {
    while (records.size() > 10) {
        records.removeLast();
    }
}