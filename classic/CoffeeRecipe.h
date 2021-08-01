#ifndef CoffeeRecipe_h
#define CoffeeRecipe_h

#include "Recipe.h"
#include "../utils/NoCopy.h"
#include <iostream>

namespace classic
{
    class CoffeeRecipe : public Recipe // 咖啡点单
    {
    public:
        CoffeeRecipe(int amountWaterMl)
            : Recipe(), m_amountWaterMl(amountWaterMl)
        {
        }

        virtual void brew() // 过滤过程
        {
            std::cout << "dripping Coffee through filter\n";
        }

        virtual int amountWaterMl() // 加水量
        {
            return m_amountWaterMl;
        }

    private:
        int m_amountWaterMl;

        NO_COPY(CoffeeRecipe);
    };
}
#endif