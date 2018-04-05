#include "delete_confirmation.h"
#include "ui_delete_confirmation.h"

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

Delete_confirmation::Delete_confirmation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Delete_confirmation)
{
    ui->setupUi(this);
}

Delete_confirmation::~Delete_confirmation()
{
    delete ui;
}

void Delete_confirmation::set_configuration_name(QString name)
{
    configuration_name = name;
    set_label_text(name);
}

void Delete_confirmation::set_label_text(QString name)
{

    ui->label->setText("Delete \"" + name + "\"?");
}

void Delete_confirmation::on_buttonBox_accepted()
{
    emit delete_configuration(configuration_name);
    close();
}
