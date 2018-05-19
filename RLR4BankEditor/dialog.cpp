
#include <iostream>
#include <QtWidgets>

#include "dialog.h"
#include "ui_consts.h"
#include "rlr4_consts.h"
#include "rlr4_bank_gen.h"

Dialog::Dialog()
{
    std::cout << "Creating Dialog" << std::endl;
    main_layout = new QVBoxLayout;
    create_meta_fields();
    create_account_data_grid();
    create_runling_data_grid();
    create_button_box();
    setLayout(main_layout);
    setWindowTitle(tr("Entity's RLR 4 Bank Editor"));
    std::cout << "Dialog Created" << std::endl;
}

void Dialog::create_meta_fields() {
    QGroupBox *meta_box = new QGroupBox(tr("Meta Information"));
    QHBoxLayout *layout = new QHBoxLayout;
    /* Handle */
    QLabel *handle_label = new QLabel(tr("Handle"));
    layout->addWidget(handle_label);
    layout->addSpacing(5);
    handle_field = new QLineEdit();
    handle_field->setPlaceholderText(tr("356683"));
    handle_field->setMaximumWidth(100);
    layout->addWidget(handle_field);
    layout->addSpacing(50);
    /* Server */
    QLabel *server_label = new QLabel(tr("Server"));
    layout->addWidget(server_label);
    layout->addSpacing(5);
    server_menu = new QComboBox;
    server_menu->addItem(tr("Americas"));
    server_menu->addItem(tr("Europe"));
    layout->addWidget(server_menu);
    /* test */
    //layout->addSpacing(50);
    //QLabel *test_label = new QLabel(tr("Test box"));
    //layout->addWidget(test_label);
    //layout->addSpacing(5);
    //MySpinBox *test_box = new MySpinBox;
    //test_box->setMaximum(10);
    //layout->addWidget(test_box);
    /* end test */
    layout->addStretch();
    /* Add to main layout */
    meta_box->setLayout(layout);
    main_layout->addWidget(meta_box);
}

template <typename F>
void Dialog::create_spinbox_column(int num_rows, QFrame *&frame,
                                   QLabel *label_arr[],
                                   QSpinBox *spinbox_arr[],
                                   int base_id, //id of first item
                                   F callback_function) {
    frame = new QFrame;
    QGridLayout *layout = new QGridLayout;
    for(int i = 0; i < num_rows; i++) {
        int id = base_id + i;
        label_arr[i] = new QLabel(tr("spinbox %1").arg(id));
        spinbox_arr[i] = new MySpinBox;
        int row = i+1;
        layout->addWidget(label_arr[i], row, 1);
        layout->addWidget(spinbox_arr[i], row, 2);
        QObject::connect(spinbox_arr[i], qOverload<int>(&QSpinBox::valueChanged), this,
                        [=](int value) { (this->*callback_function)(id, value); } );
    }
    frame->setLayout(layout);
}

void Dialog::create_account_data_grid() {
    account_data_box = new QGroupBox(tr("Account Data"));
    QHBoxLayout *account_data_layout = new QHBoxLayout;
    for (int i = 0; i < ACCOUNT_DATA_COLS; i++) {
        QFrame *frame;
        QLabel *label_arr[ACCOUNT_DATA_ROWS];
        QSpinBox *spinbox_arr[ACCOUNT_DATA_ROWS];
        create_spinbox_column(ACCOUNT_DATA_ROWS, frame, label_arr, spinbox_arr,
                              i*ACCOUNT_DATA_ROWS, account_callback);
        // set labels and max/default vals
        for (int j = 0; j < ACCOUNT_DATA_ROWS; j++) {
            int id = i*ACCOUNT_DATA_ROWS + j;
            int index = ui::account_data_indices[id];
            std::string label_str = rlr4::account_data_labels[index];
            QString label_text = QString::fromUtf8(label_str.data(), label_str.size());
            label_arr[j]->setText(label_text);
            int max_value = ui::account_data_ui_maxvals[index];
            spinbox_arr[j]->setMaximum(max_value);
            std::string tooltip = "Max value: " + std::to_string(max_value);
            label_arr[j]->setToolTip(tr(tooltip.c_str()));
            int default_value = rlr4::default_account_data[index];
            spinbox_arr[j]->setValue(default_value);
            // save spinbox in a class variable for later access
            account_fields.push_back(spinbox_arr[j]);
        }
        account_data_layout->addWidget(frame);
        if (i+1 < ACCOUNT_DATA_COLS) {
            account_data_layout->addSpacing(20);
        }
    }
    account_data_box->setLayout(account_data_layout);
    main_layout->addWidget(account_data_box);
}

void Dialog::create_runling_data_grid() {
    runling_data_box = new QGroupBox(tr("Runling Data"));
    QHBoxLayout *runling_data_layout = new QHBoxLayout;
    for (int i = 0; i < RUNLING_SLOTS; i++) {
        // initialize vector to hold this slot's runling fields
        runling_fields.push_back(std::vector<QSpinBox*>());
        QGroupBox *slot_box = new QGroupBox(tr("Slot %1").arg(i+1));
        // Runling class menu
        QComboBox *class_menu = new QComboBox;
        class_menu->addItem(tr("(Open)"));
        class_menu->addItem(tr("Zergling"));
        class_menu->addItem(tr("Baneling"));
        class_menu->addItem(tr("Hydralisk"));
        class_menu->addItem(tr("Ultralisk"));
        class_menu->addItem(tr("Roach"));
        QObject::connect(class_menu, qOverload<int>(&QComboBox::currentIndexChanged), this,
                            [=](int value) { runling_class_callback(i, value); } );
        class_menus[i] = class_menu;
        // Spinboxes for numerical data
        QFrame *slot_frame;
        QLabel *label_arr[RUNLING_DATA_ROWS];
        QSpinBox *spinbox_arr[RUNLING_DATA_ROWS];
        create_spinbox_column(RUNLING_DATA_ROWS, slot_frame, label_arr, spinbox_arr,
                            i*RUNLING_DATA_ROWS, runling_callback);
        // set labels and max/default vals
        for (int j = 0; j < RUNLING_DATA_ROWS; j++) {
            //int id = i*RUNLING_DATA_ROWS + j;
            int index = ui::runling_data_indices[j+1];
            std::string label_str = rlr4::runling_data_labels[index];
            QString label_text = QString::fromUtf8(label_str.data(), label_str.size());
            label_arr[j]->setText(label_text);
            int max_value = ui::runling_data_ui_maxvals[index];
            spinbox_arr[j]->setMaximum(max_value);
            int default_value = rlr4::default_runling_data[index];
            spinbox_arr[j]->setValue(default_value);
            // save spinbox in a class variable for later access
            runling_fields[i].push_back(spinbox_arr[j]);
        }
        // layout
        QVBoxLayout *slot_layout = new QVBoxLayout;
        slot_layout->addWidget(class_menu);
        slot_layout->addWidget(slot_frame);
        slot_box->setLayout(slot_layout);
        runling_data_layout->addWidget(slot_box);
    }
    std::cout << "test X" << std::endl;
    runling_data_box->setLayout(runling_data_layout);
    std::cout << "test Y" << std::endl;
    main_layout->addWidget(runling_data_box);
    std::cout << "test Z" << std::endl;
}

void Dialog::create_button_box() {
    QFrame *button_frame = new QFrame;
    QHBoxLayout *button_layout = new QHBoxLayout;
    button_layout->addStretch();
    QPushButton *save_button = new QPushButton("&Save to Bank", this);
    QPushButton *exit_button = new QPushButton("E&xit", this);
    connect(save_button, SIGNAL (clicked()), this, SLOT(save_callback()));
    connect(exit_button, SIGNAL (clicked()), this, SLOT(exit_callback()));
    button_layout->addWidget(save_button);
    button_layout->addWidget(exit_button);
    button_frame->setLayout(button_layout);
    main_layout->addWidget(button_frame);
}

void Dialog::account_callback(int id, int value) {
    std::cout << "account_callback(" << value << ", " << id << ")" << std::endl;
}

void Dialog::runling_callback(int id, int value) {
    std::cout << "runling_callback(" << value << ", " << id << ")" << std::endl;
}

void Dialog::runling_class_callback(int slot, int index) {
    std::cout << "runling_class_callback(" << slot << ", " << index << ")" << std::endl;
}

void Dialog::save_callback() {
    std::cout << "save_callback()" << std::endl;
    std::vector<int> account_data;
    std::vector<std::vector<int>> runling_data;
    std::vector<int> slot_numbers;
    int handle;
    int server;
    int success = collect_data(account_data, runling_data, slot_numbers, handle, server);
    if (!success) return;

    /* debug print statements */
    std::cout << "Handle: " << handle << std::endl << std::endl;
    std::cout << "Account data:" << std::endl;
    for (unsigned int i = 0; i < account_data.size(); i++) {
        std::cout << rlr4::account_data_labels[i] << ": ";
        std::cout << account_data[i] << std::endl;
    }
    std::cout << std::endl;
    assert(runling_data.size() == slot_numbers.size());
    std::cout << runling_data.size() << " runling(s) found" << std::endl;
    for (unsigned int i = 0; i < runling_data.size(); i++) {
        std::cout << "Slot " << slot_numbers[i] << ": " << bankgen::vtos(runling_data[i]) << std::endl;
    }
    std::cout << std::endl;

    /* write bank to file */
    std::ofstream fout(bankgen::OUTPUT_FILENAME, std::ofstream::out);
    fout << bankgen::generate_bank(account_data, runling_data, slot_numbers, handle, server);
    fout.close();
    std::cout << "Bank generated and saved to " << bankgen::OUTPUT_FILENAME << std::endl;
}

void Dialog::exit_callback() {
    std::cout << "exit_callback()" << std::endl;
}

int Dialog::collect_data(std::vector<int> &account_data,
                            std::vector<std::vector<int>> &runling_data,
                            std::vector<int> &slot_numbers, int &handle, int &server) {
    std::cout << "Collecting data..." << std::endl;
    // handle
    if (handle_field->text().isEmpty()) {
        std::cout << "Handle field empty" << std::endl;
        return 0;
    }
    handle = handle_field->text().toInt();
    // region
    server = server_menu->currentIndex() + 1;
    // account data
    account_data = std::vector<int>(rlr4::account_data_labels.size(), 0);
    for (unsigned int i = 0; i < ui::account_data_indices.size(); i++) {
        int index = ui::account_data_indices[i];
        int value = account_fields[i]->value();
        account_data[index] = value;
    }
    // slot numbers
    for (unsigned int i = 0; i < RUNLING_SLOTS; i++) {
        if (class_menus[i]->currentIndex() > 0) {
            slot_numbers.push_back(i+1); /* slot numbers are 1-indexed */
        }
    }
    runling_data.clear();
    for (int slot_number: slot_numbers) {
        /* first gather runling data in the order it is on the dialog */
        std::vector<int> dialog_data;
        dialog_data.push_back(class_menus[slot_number-1]->currentIndex());
        for (unsigned int i = 1; i < ui::runling_data_indices.size(); i++) {
            dialog_data.push_back(runling_fields[slot_number-1][i-1]->value());
        }
        std::cout << "Slot " << slot_number << ": " << bankgen::vtos(dialog_data) << std::endl;
        std::vector<int> ordered_data(RUNLING_DATA_ROWS+1, 0);
        for (unsigned int i = 0; i < ui::runling_data_indices.size(); i++) {
            int index = ui::runling_data_indices[i];
            ordered_data[index] = dialog_data[i];
        }
        /* compute points used and update Remaining Points in UI */
        int points_used = bankgen::calc_points_used(ordered_data);
        int points_earned = ordered_data[7]*4;
        int remaining_points = points_earned - points_used;
        if (remaining_points < 0) {
            std::cout << "Slot " << slot_number << " spent " << -remaining_points << " too many points" << std::endl;
            return 0;
        }
        runling_fields[slot_number-1][RUNLING_DATA_ROWS-1]->setValue(remaining_points);
        ordered_data[8] = remaining_points;
        runling_data.push_back(ordered_data);
    }
    std::cout << "Finished collecting data..." << std::endl;
    return 1;
}
