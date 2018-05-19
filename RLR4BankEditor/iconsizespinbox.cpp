#include <QtWidgets>

#include "iconsizespinbox.h"

IconSizeSpinBox::IconSizeSpinBox(QWidget *parent)
    : QSpinBox(parent)
{
}

int IconSizeSpinBox::valueFromText(const QString &text) const
{
    static const QRegularExpression regExp(tr("(\\d+)(\\s*[xx]\\s*\\d+)?"));
    Q_ASSERT(regExp.isValid());

    const QRegularExpressionMatch match = regExp.match(text);
    if (match.isValid())
        return match.captured(1).toInt();
    return 0;
}

QString IconSizeSpinBox::textFromValue(int value) const
{
    return tr("%1 x %1").arg(value);
}
