#ifndef Coffee_h
#define Coffee_h

#include "../utils/NoCopy.h"

namespace classic
{

    class Coffee : public CaffeineBeverage
    {
    public:
        Coffee()
            : CaffeineBeverage(m_Recipe), m_Recipe(3)
        {
        }

    private:
        CoffeeRecipe m_Recipe;
        NO_COPY(Coffee);
    };
}
#endif