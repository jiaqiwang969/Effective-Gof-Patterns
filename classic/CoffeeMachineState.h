#ifndef CoffeeMachineState_h
#define CoffeeMachineState_h

#include "../utils/NoCopy.h"
#include <string>

namespace classic
{
    class CoffeeMachine;
    // 这是前置声明(Forward declaration)
    // CoffeeMachine 需要用到CoffeeMachineState

    class CoffeeMachineState
    {
    public:
        virtual void action(CoffeeMachine &coffeeMachine, std::string const &cmd) = 0;
    };
}
#endif