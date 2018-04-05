#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"

/* Copyright Jakub Konieczny 2018
 *
 * This file is part of GBob Customizer
 *
 * GBob Customizer is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with GBob Customizer.  If not, see <http://www.gnu.org/licenses/>.
*/

enum ScanCode
{
    ESC_PRESS = 0x01,
    N1_PRESS = 0x02,
    N2_PRESS = 0x03,
    N3_PRESS = 0x04,
    N4_PRESS = 0x05,
    N5_PRESS = 0x06,
    N6_PRESS = 0x07,
    N7_PRESS = 0x08,
    N8_PRESS = 0x09,
    N9_PRESS = 0x0e,
    N0_PRESS = 0x0f,
    SCANCODE_W = 0x11,
    SCANCODE_S = 0x1F,

    SCANCODE_X = 0x2D,
    SCANCODE_Y = 0x15,
    SCANCODE_C = 0x2E,
    SCANCODE_V = 0x2F,
    SCANCODE_ESC = 0x01
};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    this->centralWidget()->setStyleSheet();
    scancode_labels = read_Scancodes("Scancodes names.txt");
    icept.moveToThread(&workerThread);
    connect(&workerThread, SIGNAL(started()), &icept, SLOT(intercepting()));
    connect(this, SIGNAL(set_reactions_map(std::unordered_map<int,BaseEffect*>)), &icept, SLOT(set_reactions_map(std::unordered_map<int,BaseEffect*>)), Qt::DirectConnection);
    startup();
    workerThread.start();
    emit set_reactions_map(reactions);
}

MainWindow::~MainWindow()
{


    delete ui;
}

void MainWindow::startup()
{
    QDir configuration_directory("configurations");
    reactions_list = configuration_directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
    if(reactions_list.size() < 1){
        std::unordered_map<int,BaseEffect*> temp;
        save_file(temp, "default");
        reactions_list = configuration_directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
    }
    ui->configurationSwitcher->addItems(reactions_list);
    QString local = reactions_list[0];
    reactions = read_file(local.toStdString());
    update_labels();
}

void MainWindow::dialog_opener(int gbob_code){
    Dialog dialog;
    dialog.setModal(true);
    dialog.set_scancode_labels(scancode_labels);
    connect(&dialog, SIGNAL(set_recordmode_on()), &icept, SLOT(record_mode_on()),Qt::DirectConnection);
    connect(&dialog, SIGNAL(set_recordmode_of()), &icept, SLOT(record_mode_of()), Qt::DirectConnection);
    connect(&icept, SIGNAL(send_recorded_stroke(InterceptionKeyStroke)), &dialog, SLOT(add_stroke(InterceptionKeyStroke)), Qt::DirectConnection);
    connect(&dialog, SIGNAL(send_recorded_press(int,SingleKeyEffect)), this, SLOT(set_recorded_key(int,SingleKeyEffect)));
    connect(&dialog, SIGNAL(send_recorded_streak(int,KeyStreakEffect)), this, SLOT(set_recorded_streak(int,KeyStreakEffect)));
    connect(&dialog, SIGNAL(send_recorded_press(int,SingleKeyEffect)), &icept, SLOT(set_recorded_key(int,SingleKeyEffect)),Qt::DirectConnection);
    connect(&dialog, SIGNAL(send_recorded_streak(int,KeyStreakEffect)), &icept, SLOT(set_recorded_streak(int,KeyStreakEffect)), Qt::DirectConnection);
//    connect(&icept, SIGNAL(sanity_check()), &dialog, SLOT(sanity_checked()));
    dialog.set_gbob_code(gbob_code);
    dialog.exec();
}

void MainWindow::set_recorded_key(int keycode, SingleKeyEffect effect)
{
    BaseEffect *temp = new SingleKeyEffect(effect);
    reactions[keycode] = temp;
    QString current_reaction_name = ui->configurationSwitcher->currentText();

    save_file(reactions, current_reaction_name.toStdString());
    update_labels();

}

void MainWindow::set_recorded_streak(int keycode, KeyStreakEffect effect)
{
    BaseEffect *temp = new KeyStreakEffect(effect);
    reactions[keycode] = temp;
    QString current_reaction_name = ui->configurationSwitcher->currentText();
    save_file(reactions, current_reaction_name.toStdString());
    update_labels();
}

void MainWindow::save_file(std::unordered_map<int, BaseEffect *> reactions, std::string filename)
{
    std::ofstream myFile;
    std::string filepath = "configurations\\" + filename;
    myFile.open(filepath);
    std::string textfile;
    std::unordered_map<int, BaseEffect*>::const_iterator got;
    for(std::pair<int, BaseEffect*> it : reactions){
       textfile += std::to_string(it.first) + "-" + it.second->file_output();
       textfile += "\n";
    }
    myFile << textfile;
    myFile.close();
}

std::unordered_map<int, BaseEffect*> MainWindow::read_file(std::string filename)
{
    std::unordered_map<int, BaseEffect*> result;
    std::string filepath = "configurations\\" + filename;
    std::ifstream myFile;
    myFile.open(filepath);
    std::string line;
    while(std::getline(myFile, line)){
        std::istringstream iss(line);
        std::string data;
        //getting gbob_keycode
        int keycode;
        std::getline(iss,data,'-');
        std::istringstream(data) >> keycode;
        //check if single key or a streeak
        std::getline(iss,data,'-');
        if(data.compare("kse") == 0){
            //get key actions tokens
            std::vector<InterceptionKeyStroke> current_streak;
            while(std::getline(iss,data,'.')){
                std::istringstream helpstream(data);
                short unsigned int code, state;
                std::string number;
                std::getline(helpstream, number,':');
                std::istringstream(number) >> code;
                helpstream >> state;
                current_streak.push_back(InterceptionKeyStroke{code,state, 0});
            }
            result[keycode] = new KeyStreakEffect(current_streak);
        }else if(data.compare("ske") == 0){

            int code;
            iss >> code;
            result[keycode] = new SingleKeyEffect(code);
        }
    }
    return result;
}

std::unordered_map<int,std::string> MainWindow::read_Scancodes(std::string filename)
{
    std::unordered_map<int,std::string> result;
    std::ifstream input;
    input.open(filename);
    std::string line;
    while(std::getline(input,line)){
        int code;
        std::string label, string_code;
        std::istringstream helpstream(line);
        std::getline(helpstream, string_code, '\t');
        std::stringstream ss;
        ss << std::hex << string_code;
        ss >> code;
        helpstream >> label;
        result[code] = label;
    }
    input.close();
    return result;
}

void MainWindow::add_configuration(QString configuration_name)
{
    QDir configuration_directory("configurations");
    std::unordered_map<int, BaseEffect*> react;
    save_file(react, configuration_name.toStdString());
    reactions_list = configuration_directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
    ui->configurationSwitcher->addItem(configuration_name);
    ui->configurationSwitcher->setCurrentIndex(ui->configurationSwitcher->findText(configuration_name));
}

void MainWindow::update_labels()
{
    if(reactions.find(0x1F) != reactions.end()){
        ui->label_s->setText(reactions[0x1F]->to_QString(scancode_labels));
    }else{
        ui->label_s->setText(QString::fromStdString(scancode_labels[0x1F]));
    }
    if(reactions.find(0x02) != reactions.end()){
        ui->label_1->setText(reactions[0x02]->to_QString(scancode_labels));
    }else{
        ui->label_1->setText(QString::fromStdString(scancode_labels[0x02]));
    }
    if(reactions.find(0x13) != reactions.end()){
        ui->label_r->setText(reactions[0x13]->to_QString(scancode_labels));
    }else{
        ui->label_r->setText(QString::fromStdString(scancode_labels[0x13]));
    }
    if(reactions.find(0x03) != reactions.end()){
        ui->label_2->setText(reactions[0x03]->to_QString(scancode_labels));
    }else{
        ui->label_2->setText(QString::fromStdString(scancode_labels[0x03]));
    }
    if(reactions.find(0x08) != reactions.end()){
        ui->label_7->setText(reactions[0x08]->to_QString(scancode_labels));
    }else{
        ui->label_7->setText(QString::fromStdString(scancode_labels[0x08]));
    }
    if(reactions.find(0x09) != reactions.end()){
        ui->label_8->setText(reactions[0x09]->to_QString(scancode_labels));
    }else{
        ui->label_8->setText(QString::fromStdString(scancode_labels[0x09]));
    }
    if(reactions.find(0x2A) != reactions.end()){
        ui->label_shift->setText(reactions[0x2A]->to_QString(scancode_labels));
    }else{
        ui->label_shift->setText(QString::fromStdString(scancode_labels[0x2A]));
    }
    if(reactions.find(0x06) != reactions.end()){
        ui->label_5->setText(reactions[0x06]->to_QString(scancode_labels));
    }else{
        ui->label_5->setText(QString::fromStdString(scancode_labels[0x06]));
    }
    if(reactions.find(0x0B) != reactions.end()){
        ui->label_0->setText(reactions[0x0B]->to_QString(scancode_labels));
    }else{
        ui->label_0->setText(QString::fromStdString(scancode_labels[0x0B]));
    }
    if(reactions.find(0x39) != reactions.end()){
        ui->label_space->setText(reactions[0x39]->to_QString(scancode_labels));
    }else{
        ui->label_space->setText(QString::fromStdString(scancode_labels[0x39]));
    }
    if(reactions.find(0x3A) != reactions.end()){
        ui->label_caps_lock->setText(reactions[0x3A]->to_QString(scancode_labels));
    }else{
        ui->label_caps_lock->setText(QString::fromStdString(scancode_labels[0x3A]));
    }
    if(reactions.find(0x1D) != reactions.end()){
        ui->label_ctrl->setText(reactions[0x1D]->to_QString(scancode_labels));
    }else{
        ui->label_ctrl->setText(QString::fromStdString(scancode_labels[0x1D]));
    }
    if(reactions.find(0x38) != reactions.end()){
        ui->label_alt->setText(reactions[0x38]->to_QString(scancode_labels));
    }else{
        ui->label_alt->setText(QString::fromStdString(scancode_labels[0x38]));
    }

    if(reactions.find(0x0F) != reactions.end()){
        ui->label_tab->setText(reactions[0x0F]->to_QString(scancode_labels));
    }else{
        ui->label_tab->setText(QString::fromStdString(scancode_labels[0x0F]));
    }
    if(reactions.find(0x01) != reactions.end()){
        ui->label_esc->setText(reactions[0x01]->to_QString(scancode_labels));
    }else{
        ui->label_esc->setText(QString::fromStdString(scancode_labels[0x01]));
    }
    if(reactions.find(0x0A) != reactions.end()){
        ui->label_9->setText(reactions[0x0A]->to_QString(scancode_labels));
    }else{
        ui->label_9->setText(QString::fromStdString(scancode_labels[0x0A]));
    }
    if(reactions.find(0x05) != reactions.end()){
        ui->label_4->setText(reactions[0x05]->to_QString(scancode_labels));
    }else{
        ui->label_4->setText(QString::fromStdString(scancode_labels[0x05]));
    }
    if(reactions.find(0x04) != reactions.end()){
        ui->label_3->setText(reactions[0x04]->to_QString(scancode_labels));
    }else{
        ui->label_3->setText(QString::fromStdString(scancode_labels[0x04]));
    }
    if(reactions.find(0x07) != reactions.end()){
        ui->label_6->setText(reactions[0x07]->to_QString(scancode_labels));
    }else{
        ui->label_6->setText(QString::fromStdString(scancode_labels[0x07]));
    }
    if(reactions.find(0x10) != reactions.end()){
        ui->label_q->setText(reactions[0x10]->to_QString(scancode_labels));
    }else{
        ui->label_q->setText(QString::fromStdString(scancode_labels[0x10]));
    }
    if(reactions.find(0x1E) != reactions.end()){
        ui->label_a->setText(reactions[0x1E]->to_QString(scancode_labels));
    }else{
        ui->label_a->setText(QString::fromStdString(scancode_labels[0x1E]));
    }
    if(reactions.find(0x11) != reactions.end()){
        ui->label_w->setText(reactions[0x11]->to_QString(scancode_labels));
    }else{
        ui->label_w->setText(QString::fromStdString(scancode_labels[0x11]));
    }
    if(reactions.find(0x20) != reactions.end()){
        ui->label_d->setText(reactions[0x20]->to_QString(scancode_labels));
    }else{
        ui->label_d->setText(QString::fromStdString(scancode_labels[0x20]));
    }if(reactions.find(0x0E) != reactions.end()){
        ui->label_backspace->setText(reactions[0x0E]->to_QString(scancode_labels));
    }else{
        ui->label_backspace->setText(QString::fromStdString(scancode_labels[0x0E]));
    }

}


void MainWindow::on_configurationSwitcher_currentIndexChanged(const QString &arg1)
{
    reactions = read_file(arg1.toStdString());
    emit set_reactions_map(reactions);
    update_labels();
}

void MainWindow::on_set_s_clicked()
{

    dialog_opener(0x1F);
}

void MainWindow::on_set_1_clicked()
{
     dialog_opener(0x02);
}

void MainWindow::on_set_6_clicked()
{
    dialog_opener(0X07);
}


void MainWindow::on_set_alt_clicked()
{
    dialog_opener(0x38);
}


void MainWindow::on_set_r_clicked()
{
    dialog_opener(0x13);
}

void MainWindow::on_set_q_clicked()
{
    dialog_opener(0x10);
}

void MainWindow::on_set_2_clicked()
{
    dialog_opener(0x03);
}

void MainWindow::on_set_7_clicked()
{
    dialog_opener(0x08);
}

void MainWindow::on_set_3_clicked()
{
    dialog_opener(0x04);
}

void MainWindow::on_set_8_clicked()
{
    dialog_opener(0x09);
}

void MainWindow::on_set_shift_clicked()
{
    dialog_opener(0x2A);
}

void MainWindow::on_set_4_clicked()
{
    dialog_opener(0x05);
}

void MainWindow::on_set_9_clicked()
{
    dialog_opener(0x0A);
}

void MainWindow::on_set_5_clicked()
{
    dialog_opener(0x06);
}

void MainWindow::on_set_0_clicked()
{
    dialog_opener(0x0B);
}

void MainWindow::on_set_esc_clicked()
{
    dialog_opener(0x01);
}

void MainWindow::on_set_tab_clicked()
{
    dialog_opener(0x0F);
}

void MainWindow::on_set_space_clicked()
{
    dialog_opener(0x39);
}

void MainWindow::on_set_caps_lock_clicked()
{
    dialog_opener(0x3A);
}

void MainWindow::on_set_ctrl_clicked()
{
    dialog_opener(0x1D);
}

void MainWindow::on_add_new_clicked()
{
    new_configuration dialog;
    dialog.setModal(true);
    connect(&dialog, SIGNAL(send_new_configuration_name(QString)), this, SLOT(add_configuration(QString)));
    dialog.exec();
}

void MainWindow::on_set_d_clicked()
{
    dialog_opener(0x20);
}

void MainWindow::on_set_backspace_clicked()
{
    dialog_opener(0x0E);
}

void MainWindow::on_delete_conf_clicked()
{
    Delete_confirmation dialog;
    dialog.setModal(true);
    connect(this, SIGNAL(confirm_delete(QString)), &dialog, SLOT(set_configuration_name(QString)));
    connect(&dialog, SIGNAL(delete_configuration(QString)), this, SLOT(delete_conf(QString)));
    emit confirm_delete(ui->configurationSwitcher->currentText());
    dialog.exec();
}

void MainWindow::delete_conf(QString configuration_name)
{
    QDir directory("configurations");
    directory.remove(configuration_name);
    ui->configurationSwitcher->clear();
    startup();
}

void MainWindow::on_set_a_clicked()
{
    dialog_opener(0x1E);
}

void MainWindow::on_set_w_clicked()
{
    dialog_opener(0x11);
}
