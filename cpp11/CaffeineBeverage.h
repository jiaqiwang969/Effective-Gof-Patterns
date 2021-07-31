#ifndef cpp11_CaffeineBeverage_h
#define cpp11_CaffeineBeverage_h

#include "Recipe.h"
#include "../utils/NoCopyNoMove.h"
#include <functional>
#include <iostream>

namespace cpp11
{
    class CaffeineBeverage
    {
    public:
        //  构造函数1
        CaffeineBeverage(Recipe Recipe)
            : m_brew(Recipe.brew), m_addCondiments(Recipe.addCondiments)
        {
        }

        // 构造函数2
        CaffeineBeverage(std::function<void()> brew, std::function<void()> addCondiments)
            : m_brew(brew), m_addCondiments(addCondiments)
        {
        }

        void prepareRecipe()
        {
            boilWater();
            m_brew();
            pourInCup();
            m_addCondiments();
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

        std::function<void()> m_brew;
        std::function<void()> m_addCondiments;

        NO_COPY_NO_MOVE(CaffeineBeverage);
    };
}

#endif