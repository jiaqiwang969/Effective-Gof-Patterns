#ifndef CaffeineBeverage_h
#define CaffeineBeverage_h

#include "../utils/NoCopy.h"
#include <iostream>

namespace classic
{
    class CaffeineBeverage // 咖啡机
    {
    public:
        CaffeineBeverage(Recipe &Recipe)
            : m_Recipe(Recipe)
        {
        }

        virtual ~CaffeineBeverage()
        {
        }

        void prepareRecipe() // 制作过程
        {
            boilWater();
            m_Recipe.brew(); // 跟对象有关的动作
            pourInCup();
            m_Recipe.addCondiments(); // 跟对象有关的动作
        }

    private:
        void boilWater()
        {
            std::cout << "boil water\n";
        }

        void pourInCup()
        {
            std::cout << "pour in cup\n";
        }

        Recipe &m_Recipe;

        NO_COPY(CaffeineBeverage);
    };
}

#endif
