#ifndef ICEPT_H
#define ICEPT_H

#include <QObject>
#include <unordered_map>
#include "interception.h"
#include "singlekeyeffect.h"
#include "keystreakeffect.h"
#include "baseeffect.h"
#include <iostream>



class IceptWorker : public QObject
{
    Q_OBJECT
protected:


public:
    IceptWorker();

    public:



private:
    InterceptionContext context;
    InterceptionDevice device;
    bool keepWorking;
    bool recordMode;
    std::unordered_map<int, BaseEffect*> reactions;
    void key_stroke(InterceptionKeyStroke &stroke);

public slots:
    void record_mode_on();
    void record_mode_of();
    void set_reactions_map(std::unordered_map<int, BaseEffect*> reactions);
    void intercepting();
    void set_recorded_streak(int keycode, KeyStreakEffect effect);
    void set_recorded_key(int keycode, SingleKeyEffect effect);

signals:
    void send_recorded_stroke(InterceptionKeyStroke current_stroke);
    void sanity_check();

};

#endif // ICEPT_H
