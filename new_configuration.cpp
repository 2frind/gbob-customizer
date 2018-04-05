#include "new_configuration.h"
#include "ui_new_configuration.h"

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

new_configuration::new_configuration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::new_configuration)
{
    ui->setupUi(this);
}

new_configuration::~new_configuration()
{
    delete ui;
}

void new_configuration::on_buttonBox_accepted()
{
    QString result = ui->lineEdit->text();
    if (!check_if_file_exists(result)) emit send_new_configuration_name(result);
    close();
}

bool new_configuration::check_if_file_exists(QString filename)
{
    QDir configuration_directory("configurations");
    QStringList reactions_list = configuration_directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
    return reactions_list.contains(filename + ".txt");
}
