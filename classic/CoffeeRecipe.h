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
        CoffeeRecipe(int minutes)
            : Recipe(), m_minutes(minutes)
        {
        }

        virtual void brew() // 过滤过程
        {
            std::cout << m_minutes << "min dripping Coffee through filter\n";
        }

        virtual void addCondiments() // 加调味料
        {
            std::cout << "Adding Sugar and Milk\n";
        }

    private:
        int m_minutes;

        NO_COPY(CoffeeRecipe);
    };
}
#endif