#include "dialog.h"
#include "ui_dialog.h"

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

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::add_stroke(InterceptionKeyStroke stroke){
    streak.push_back(stroke);
    if(stroke.state == INTERCEPTION_KEY_DOWN){
        if(streak.size() > 1) labelText += "+";
        if(labels.find(stroke.code) != labels.end()){
            labelText += QString::fromStdString(labels[stroke.code]);
        }else{
            labelText += QString::number(stroke.code);
        }
        ui->label->setText(labelText);
    }
}

void Dialog::accept(){
    //fixStreak(streak);
    if(streak.size() < 3 ){
        SingleKeyEffect effect = SingleKeyEffect(streak[0]);
        emit send_recorded_press(gbob_keycode, SingleKeyEffect(streak[0]));
    }else{
        emit send_recorded_streak(gbob_keycode, KeyStreakEffect(streak));
    }
    close();
}

void Dialog::set_gbob_code(int code){
    gbob_keycode = code;
    emit set_recordmode_on();
}

void Dialog::set_scancode_labels(std::unordered_map<int, std::string> labels)
{

    this->labels = labels;
}

void Dialog::on_buttonBox_accepted()
{

    emit set_recordmode_of();
}


void Dialog::on_buttonBox_rejected()
{
    emit set_recordmode_of();
}

bool Dialog::event(QEvent *event)
{
    switch (event->type()) {
        case QEvent::WindowDeactivate :
            emit set_recordmode_of();
            break;
    case QEvent::WindowActivate :
            emit set_recordmode_on();
            break;
        default:

            break;
    }
    return QDialog::event(event);
}
