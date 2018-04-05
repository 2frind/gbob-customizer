#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "interception.h"
#include <vector>
#include <set>
#include <unordered_map>
#include "iceptworker.h"
#include "singlekeyeffect.h"
#include "keystreakeffect.h"
#include "baseeffect.h"
#include "QThread"
#include <iostream>
#include <fstream>
#include <sstream>
#include "QDir"
#include "QStringList"
#include "new_configuration.h"
#include "delete_confirmation.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void set_recorded_streak(int keycode, KeyStreakEffect effect);
    void set_recorded_key(int keycode, SingleKeyEffect effect);


signals:
    void set_reactions_map(std::unordered_map<int, BaseEffect*> reactions);
    void confirm_delete(QString name);
    
private slots:
    void on_configurationSwitcher_currentIndexChanged(const QString &arg1);
    void add_configuration(QString configuration_name);
    void delete_conf(QString configuration_name);

    void on_set_s_clicked();
    void on_set_1_clicked();
    void on_set_6_clicked();
    void on_set_alt_clicked();
    void on_set_r_clicked();
    void on_set_q_clicked();
    void on_set_2_clicked();
    void on_set_7_clicked();
    void on_set_3_clicked();
    void on_set_8_clicked();
    void on_set_shift_clicked();
    void on_set_4_clicked();
    void on_set_9_clicked();
    void on_set_5_clicked();
    void on_set_0_clicked();
    void on_set_esc_clicked();
    void on_set_tab_clicked();
    void on_set_space_clicked();
    void on_set_caps_lock_clicked();
    void on_set_ctrl_clicked();
    void on_set_d_clicked();
    void on_add_new_clicked();
    void on_set_backspace_clicked();
    void on_delete_conf_clicked();

    void on_set_a_clicked();

    void on_set_w_clicked();

private:
    std::string configuration_path = "/configurations";
    Ui::MainWindow *ui;
    IceptWorker icept;
    QThread workerThread;
    std::unordered_map<int, BaseEffect*> reactions;
    QStringList reactions_list;
    std::unordered_map<int, std::string> scancode_labels;
    void startup();
    void dialog_opener(int gbob_code);
    void save_file(std::unordered_map<int,BaseEffect*> reactions, std::string filename);
    std::unordered_map<int,BaseEffect*> read_file(std::string filename);
    std::unordered_map<int,std::string> read_Scancodes(std::string filename);
    void update_labels();


};

#endif // MAINWINDOW_H
