#ifndef SINGLEKEYEFFECT_H
#define SINGLEKEYEFFECT_H
#include "baseeffect.h"

class SingleKeyEffect : virtual public BaseEffect
{
private:
    int _keyCode;
public:
    SingleKeyEffect();
    SingleKeyEffect(int keyCode);
    SingleKeyEffect(InterceptionKeyStroke stroke);
    SingleKeyEffect(const SingleKeyEffect& copied);
    ~SingleKeyEffect();
    void keyAction(InterceptionContext context,	InterceptionDevice device, InterceptionKeyStroke &stroke);
    std::string file_output();
    QString to_QString(std::unordered_map<int, std::string> scancode_labels);
    int get_keyCode();
};

#endif // SINGLEKEYEFFECT_H
