#include "keystreakeffect.h"

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


KeyStreakEffect::KeyStreakEffect()
{
}

KeyStreakEffect::KeyStreakEffect(std::vector<InterceptionKeyStroke> streak)
{
    for (unsigned int i = 0; i < streak.size(); ++i) {
        strokes.push_back(InterceptionKeyStroke(streak[i]));
    }
}

KeyStreakEffect::~KeyStreakEffect()
{
}

void KeyStreakEffect::keyAction(InterceptionContext context, InterceptionDevice device, InterceptionKeyStroke & stroke)
{
    if (stroke.state == 0) {
        for (unsigned int i = 0; i < strokes.size(); ++i) {
            interception_send(context, device, (InterceptionStroke *)&strokes[i], 1);
        }
    }
}

std::string KeyStreakEffect::file_output()
{
    std::string fileString = "kse-";
    for(unsigned int i = 0 ; i < strokes.size() ; i++){
        if(i != 0) fileString += ".";
        fileString += std::to_string(strokes[i].code) + ":" + std::to_string(strokes[i].state);
    }
    return  fileString;
}

QString KeyStreakEffect::to_QString(std::unordered_map<int, std::string> scancode_labels)
{
    QString result;
    for(unsigned int i = 0 ; i < strokes.size() ; ++i){
        if(strokes[i].state != INTERCEPTION_KEY_UP){
            if(i > 0) result += "+";
            result += QString::fromStdString(scancode_labels[strokes[i].code]);
        }
    }
    return result;
}


