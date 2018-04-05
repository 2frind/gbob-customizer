#include "mainwindow.h"
#include <QApplication>
#include "interception.h"

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

Q_DECLARE_METATYPE(InterceptionKeyStroke)
Q_DECLARE_METATYPE(SingleKeyEffect)
Q_DECLARE_METATYPE(KeyStreakEffect)

int main(int argc, char *argv[])
{
    std::cout << "Started\n";
    qRegisterMetaType<InterceptionKeyStroke>();
    qRegisterMetaType<SingleKeyEffect>();
    qRegisterMetaType<KeyStreakEffect>();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
