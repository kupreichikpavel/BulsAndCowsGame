#pragma once

#include <QDialog>
#include <QVector>

class QTableWidget;

struct Record {
    int attempts;
    QString name;
};

class RecordsDialog : public QDialog {
    Q_OBJECT

public:
    explicit RecordsDialog(const QVector<Record>& records, QWidget *parent = nullptr);

private:
    QTableWidget* table;
};