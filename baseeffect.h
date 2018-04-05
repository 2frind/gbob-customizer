#ifndef BASEEFFECT_H
#define BASEEFFECT_H
#include <interception.h>
#include <string>
#include "QString"
#include <unordered_map>
#include <unordered_map>


class BaseEffect
{
public:
    BaseEffect();
    ~BaseEffect();
    virtual void keyAction(InterceptionContext context, InterceptionDevice device, InterceptionKeyStroke &stroke) = 0;
    virtual std::string file_output() = 0;
    virtual QString to_QString(std::unordered_map<int, std::string> scancode_labels) = 0;
};

#endif // BASEEFFECT_H
