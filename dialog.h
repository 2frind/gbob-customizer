#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "interception.h"
#include "singlekeyeffect.h"
#include "keystreakeffect.h"
#include <vector>
#include <iostream>
#include <unordered_map>



namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    void set_gbob_code(int code);
    void set_scancode_labels(std::unordered_map<int,std::string> labels);
    ~Dialog();

protected:
    bool event(QEvent *event);

private:
    Ui::Dialog *ui;
    std::vector<InterceptionKeyStroke> streak;
    QString labelText;
    std::unordered_map<int,std::string> labels;
    int gbob_keycode;
    void accept();


public slots:
    void add_stroke(InterceptionKeyStroke stroke);

signals:
    void set_recordmode_on();
    void set_recordmode_of();
    void send_recorded_streak(int key, KeyStreakEffect effect);
    void send_recorded_press(int key, SingleKeyEffect effect);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // DIALOG_H
