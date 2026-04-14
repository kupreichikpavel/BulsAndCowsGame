#pragma once

#include <QString>
#include <QVector>

struct Record {
    QString name;
    int attempts = 0;
};

class RecordManager {
public:
    explicit RecordManager(QString fileName = "records.txt");

    void load();
    void save() const;

    const QVector<Record>& getRecords() const;
    bool qualifies(int attempts) const;
    void addRecord(const QString& name, int attempts);

private:
    QString filePath;
    QVector<Record> records;

    void sortAndTrim();
};