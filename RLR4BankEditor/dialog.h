#ifndef DIALOG_H
#define DIALOG_H

#include <vector>
#include <string>

#include <QDialog>

QT_BEGIN_NAMESPACE
class QAction;
class QComboBox;
class QDialogButtonBox;
class QFrame;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QMenuBar;
class QPushButton;
class QTextEdit;
class QVBoxLayout;
class QSpinBox;
QT_END_NAMESPACE

#include "myspinbox.h"
//#include "iconsizespinbox.h"

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog();

private: /* constant defintions */
    const static int ACCOUNT_DATA_ROWS = 4;
    const static int ACCOUNT_DATA_COLS = 8;
    const static int RUNLING_DATA_ROWS = 8;
    const static int RUNLING_SLOTS = 8;

public slots:
    void account_callback(int id, int value);
    void runling_callback(int id, int value);
    void runling_class_callback(int slot, int index);
    void save_callback();
    void exit_callback();

private:
    void create_meta_fields();
    void create_account_data_grid();
    void create_runling_data_grid();
    void create_button_box();

    /* helper method to create column of spinboxes */
    template <typename F>
    void create_spinbox_column(int num_rows, QFrame *&frame,
                                QLabel *label_arr[], QSpinBox *spinbox_arr[],
                                int base_id, F callback_function);

    /* collect data from ui to vectors
     * ready to pass into bank gen function
     * returns 1 if sucess, 0 if failure (missing info) */
    int collect_data(std::vector<int> &account_data,
                      std::vector<std::vector<int>> &runling_data,
                      std::vector<int> &slot_numbers, int &handle, int &server);


    /* layout */
    QVBoxLayout *main_layout;
    QGroupBox *account_data_box;
    QGroupBox *runling_data_box;

    /* data-containing UI elements */
    QLineEdit *handle_field;
    QComboBox* server_menu;
    std::vector<QSpinBox*> account_fields;
    QComboBox* class_menus[RUNLING_SLOTS];
    std::vector<std::vector<QSpinBox*>> runling_fields;
};

#endif // DIALOG_H
