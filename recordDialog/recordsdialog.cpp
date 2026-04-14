#include "recordsdialog.h"

#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>

RecordsDialog::RecordsDialog(const QVector<Record>& records, QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Рекорды");
    resize(400, 300);

    auto *layout = new QVBoxLayout(this);

    table = new QTableWidget(this);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"Место", "Попытки", "Имя"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::NoSelection);

    table->setRowCount(records.size());

    for (int i = 0; i < records.size(); ++i) {
        table->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(records[i].attempts)));
        table->setItem(i, 2, new QTableWidgetItem(records[i].name));
    }

    layout->addWidget(table);
}