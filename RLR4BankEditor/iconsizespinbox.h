#ifndef ICONSIZESPINBOX_H
#define ICONSIZESPINBOX_H

#include <QSpinBox>

class IconSizeSpinBox : public QSpinBox
{
    Q_OBJECT

public:
    explicit IconSizeSpinBox(QWidget *parent = Q_NULLPTR);

    int valueFromText(const QString &text) const override;
    QString textFromValue(int value) const override;
};

#endif
