#ifndef Coffee_h
#define Coffee_h

#include "../utils/NoCopy.h"

namespace classic
{

    class Coffee : public CaffeineBeverage
    {
    public:
        Coffee()
            : CaffeineBeverage(m_Recipe), m_Recipe(3) // 默认加水量，工厂触发
        {
        }

    private:
        CoffeeRecipe m_Recipe;
        NO_COPY(Coffee);
    };
}
#endif