#ifndef TeaRecipe_h
#define TeaRecipe_h

#include "../utils/NoCopy.h"
#include <iostream>

namespace classic
{
    class TeaRecipe : public Recipe
    {
    public:
        TeaRecipe(int amountWaterMl)
            : Recipe(), m_amountWaterMl(amountWaterMl)
        {
        }

        virtual void brew()
        {
            std::cout << "steeping Tea through filter\n";
        }

        virtual int amountWaterMl()
        {
            return m_amountWaterMl;
        }

    private:
        int m_amountWaterMl;

        NO_COPY(TeaRecipe);
    };
}
#endif