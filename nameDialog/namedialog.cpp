#include "namedialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

NameDialog::NameDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Введите имя");
    setModal(true);
    resize(300, 120);

    auto *mainLayout = new QVBoxLayout(this);

    auto *label = new QLabel("Имя:");
    mainLayout->addWidget(label);

    nameEdit = new QLineEdit(this);
    mainLayout->addWidget(nameEdit);

    auto *buttonsLayout = new QHBoxLayout();

    auto *okButton = new QPushButton("OK", this);
    auto *cancelButton = new QPushButton("Cancel", this);

    buttonsLayout->addWidget(cancelButton);
    buttonsLayout->addWidget(okButton);

    mainLayout->addLayout(buttonsLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

QString NameDialog::getName() const {
    return nameEdit->text().trimmed();
}