#ifndef MYSPINBOX_H
#define MYSPINBOX_H


#include <QSpinBox>
#include <QValidator>

class MySpinBox : public QSpinBox
{
    Q_OBJECT

public:
    explicit MySpinBox(QWidget *parent = Q_NULLPTR);

   QValidator::State validate(QString &text, int &pos) const override;

    void fixup(QString &input) const override;
};

#endif // MYSPINBOX_H
