#ifndef cpp11_CaffeineBeverage_h
#define cpp11_CaffeineBeverage_h

//#include "Recipe.h"
#include "Condiment.h"
#include "../utils/NoCopyNoMove.h"
#include <functional>
#include <iostream>
#include <string>

namespace cpp11
{
    class CaffeineBeverage
    {
    public:
        //  构造函数1
        // CaffeineBeverage(Recipe Recipe)
        //     : m_brew(Recipe.brew), m_addCondiments(Recipe.addCondiments)
        // {
        // }

        // 构造函数2
        CaffeineBeverage(std::function<void()> brew, std::function<void()> addCondiments)
            : m_brew(brew), m_addCondiments(addCondiments), m_description("CaffeineBeverage"), m_condiment()
        {
        }

        void prepareRecipe() const
        {
            boilWater();
            m_brew();
            pourInCup();
            m_addCondiments();
            addCondiments();
        }

        void condiments(Condiment condiment)
        {
            m_condiment = condiment;
        }

        std::string description() const
        {
            std::string condiments = m_condiment.description ? m_condiment.description() : "---";
            return m_description + " : " + condiments;
        }

    private:
        void boilWater() const
        {
            std::cout << "boil water\n";
        }

        void pourInCup() const
        {
            std::cout << "pour in cup\n";
        }

        void addCondiments() const
        {
            if (m_condiment.description)
                std::cout << m_condiment.description() << '\n';
        }

        std::function<void()> m_brew;
        std::function<void()> m_addCondiments;
        std::string m_description;
        Condiment m_condiment;

        NO_COPY_NO_MOVE(CaffeineBeverage);
    };
}

#endif