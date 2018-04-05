#include "iceptworker.h"

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

IceptWorker::IceptWorker()
{

    context = interception_create_context();
    keepWorking = true;
    recordMode = false;

}

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



void IceptWorker::key_stroke(InterceptionKeyStroke &stroke) {
    interception_send(context, device, (InterceptionStroke *)&stroke, 1);
}

bool operator == (const InterceptionKeyStroke &first, const InterceptionKeyStroke &second)
{
    return first.code == second.code && first.state == second.state;
}

bool operator != (const InterceptionKeyStroke &first, const InterceptionKeyStroke &second)
{
    return !(first == second);
}

void IceptWorker::intercepting(){
    InterceptionKeyStroke stroke;
        InterceptionKeyStroke prevStroke;
        std::unordered_map<int, BaseEffect*>::const_iterator got;

        wchar_t hardware_id[500];
        wchar_t gbobID[] = L"HID\\VID_0000&PID_5710&REV_0200";


        interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN | INTERCEPTION_FILTER_KEY_UP);

        while (interception_receive(context, device = interception_wait(context), (InterceptionStroke *)&stroke, 1) > 0)
        {
            size_t length = interception_get_hardware_id(context, device, hardware_id, sizeof(hardware_id));
            if (!recordMode){
                if (wcscmp(hardware_id, gbobID) == 0) {
                    {
                        if (reactions.find(stroke.code) != reactions.end()) {
                            reactions[stroke.code]->keyAction(context, device, stroke);
                        }else{
                            interception_send(context, device, (InterceptionStroke *)&stroke, 1);
                        }
                    }
                }
                else
                {
                    interception_send(context, device, (InterceptionStroke *)&stroke, 1);
                }

            }else{
                if(prevStroke != stroke){
                    emit send_recorded_stroke(stroke);
                }
            }

            prevStroke = stroke;
        }

}

void IceptWorker::record_mode_on(){
    recordMode = true;
}

void IceptWorker::record_mode_of(){
    recordMode = false;
}

void IceptWorker::set_reactions_map(std::unordered_map<int, BaseEffect*> reactions){
    this->reactions = reactions;
}

void IceptWorker::set_recorded_key(int keycode, SingleKeyEffect effect)
{
    BaseEffect *temp = new SingleKeyEffect(effect);
    reactions[keycode] = temp;
}

void IceptWorker::set_recorded_streak(int keycode, KeyStreakEffect effect)
{
    BaseEffect *temp = new KeyStreakEffect(effect);
    reactions[keycode] = temp;
}
