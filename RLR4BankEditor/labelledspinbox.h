#ifndef LABELLEDSPINBOX_H
#define LABELLEDSPINBOX_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QFrame;
class QLabel;
class QSpinBox;
QT_END_NAMESPACE

class LabelledSpinBox
{
public:
    LabelledSpinBox(const QString &label_text);

    QFrame *frame;
    QLabel *label;
    QSpinBox *spinbox;

};

#endif // LABELLEDSPINBOX_H
