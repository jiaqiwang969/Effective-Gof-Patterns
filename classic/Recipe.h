#ifndef Recipe_h
#define Recipe_h

#include "../utils/NoCopy.h"

namespace classic
{
    class Recipe // 收据
    {
    public:
        virtual void brew() = 0;          // 继承
        virtual int amountWaterMl() = 0; // 继承
    };

}
#endif