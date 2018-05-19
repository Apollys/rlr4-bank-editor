#ifndef SPINBOXCOLUMN_H
#define SPINBOXCOLUMN_H

#include <QDialog>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
class QFrame;
class QLabel;
class QSpinBox;
class QGridLayout;
QT_END_NAMESPACE

class SpinBoxColumn
{
public:
    template<typename F>
    SpinBoxColumn(int num_rows, F callback_function);

    int num_rows;
    QFrame *frame;
    QLabel **labels;
    QSpinBox **spinboxes;
};

#endif // SPINBOXCOLUMN_H
