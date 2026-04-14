#pragma once

#include <QDialog>

class QLineEdit;

class NameDialog : public QDialog {
    Q_OBJECT

public:
    explicit NameDialog(QWidget *parent = nullptr);
    QString getName() const;

private:
    QLineEdit* nameEdit;
};