#include "spinboxcolumn.h"

template <typename F>
SpinBoxColumn::SpinBoxColumn(int num_rows, F callback_function) {
    this->num_rows = num_rows;
    frame = new QFrame();
    QGridLayout *layout = new QGridLayout;
    QLabel *label_arr[num_rows];
    QSpinBox *spinbox_arr[num_rows];
    for(int i = 0; i < num_rows; i++) {
        int row = i+1;
        QString label_text = QObject::tr("Line %1:").arg(row);
        label_arr[i] = new QLabel(label_text);
        spinbox_arr[i] = new QSpinBox;
        layout->addWidget(label_arr[i], row, 1);
        layout->addWidget(spinbox_arr[i], row, 2);
    }
    frame->setLayout(layout);
    /* QObject::connect(spinbox_arr[7], SIGNAL(valueChanged(int)),
                        this, SLOT(callback_function(int, int id=7)));*/
}
