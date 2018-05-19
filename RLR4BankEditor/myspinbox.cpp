
#include <QtWidgets>
#include <iostream>
#include "myspinbox.h"

MySpinBox::MySpinBox(QWidget *parent) : QSpinBox(parent) {}

QValidator::State MySpinBox::validate(QString &text, int &pos) const {
    bool valid_int = false;
    int value = text.toInt(&valid_int);
    if (valid_int) {
        if (value >= this->minimum() && value <= this->maximum()) {
            return QValidator::Acceptable;
        }
        return QValidator::Intermediate;
    }
    return QSpinBox::validate(text, pos);
}

void MySpinBox::fixup(QString &input) const {
    bool valid_int = false;
    int value = input.toInt(&valid_int);
    if (valid_int) {
        value = qBound(this->minimum(), value, this->maximum());
        input = QString::number(value);
    }
    else {
        QSpinBox::fixup(input);
    }
}
