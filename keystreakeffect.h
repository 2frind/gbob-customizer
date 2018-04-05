#ifndef KEYSTREAKEFFECT_H
#define KEYSTREAKEFFECT_H
#include <vector>
#include "baseeffect.h"
#include "qstring.h"

class KeyStreakEffect : virtual public BaseEffect
{
private:
    std::vector<InterceptionKeyStroke> strokes;
public:

    KeyStreakEffect();
    KeyStreakEffect(std::vector<InterceptionKeyStroke> streak);
    ~KeyStreakEffect();
    void keyAction(InterceptionContext context, InterceptionDevice device, InterceptionKeyStroke &stroke);
    std::string file_output();
    QString to_QString(std::unordered_map<int, std::string> scancode_labels);
};

#endif // KEYSTREAKEFFECT_H
