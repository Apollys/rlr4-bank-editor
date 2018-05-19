#include <QtWidgets>
#include "labelledspinbox.h"


/*
    QGroupBox *groupbox;
    QLabel *label;
    QSpinBox *spinbox;
 */

LabelledSpinBox::LabelledSpinBox(const QString &label_text)
{
    frame = new QFrame();
    QHBoxLayout *layout = new QHBoxLayout;
    label = new QLabel(label_text);
    spinbox = new QSpinBox;
    layout->addWidget(label);
    layout->addWidget(spinbox);
    frame->setLayout(layout);
}
