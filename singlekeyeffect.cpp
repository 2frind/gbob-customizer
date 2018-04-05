#include "singlekeyeffect.h"

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

SingleKeyEffect::SingleKeyEffect()
{

}

SingleKeyEffect::SingleKeyEffect(int keyCode)
{
    _keyCode = keyCode;
}

SingleKeyEffect::SingleKeyEffect(InterceptionKeyStroke stroke)
{
    _keyCode = stroke.code;
}

SingleKeyEffect::SingleKeyEffect(const SingleKeyEffect& copied)
{
    _keyCode = copied._keyCode;
}

SingleKeyEffect::~SingleKeyEffect(){}

void SingleKeyEffect::keyAction(InterceptionContext context, InterceptionDevice device, InterceptionKeyStroke &stroke)
{
    stroke.code = _keyCode;
    interception_send(context, device, (InterceptionStroke *)&stroke, 1);
}

std::string SingleKeyEffect::file_output()
{
    std::string fileString = "ske-";
    fileString += std::to_string(_keyCode);
    return fileString;
}

QString SingleKeyEffect::to_QString(std::unordered_map<int, std::string> scancode_labels)
{
    QString result = QString::fromStdString(scancode_labels[_keyCode]);
    return result;
}

int SingleKeyEffect::get_keyCode(){
    return _keyCode;
}
