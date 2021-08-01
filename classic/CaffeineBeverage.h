#ifndef CaffeineBeverage_h
#define CaffeineBeverage_h

#include "../utils/NoCopy.h"
#include <iostream>
#include "Recipe.h"
#include "Condiment.h"

namespace classic
{
    class CaffeineBeverage // 咖啡机
    {
    public:
        CaffeineBeverage(Recipe &recipe)
            : m_recipe(recipe), m_condiments(0)
        {
        }

        virtual ~CaffeineBeverage()
        {
            if (m_condiments)
                delete m_condiments;
        }

        void prepareRecipe() // 制作过程
        {
            boilWater(m_recipe.amountWaterMl());
            m_recipe.brew(); // 跟对象有关的动作
            pourInCup();
            //m_recipe.addCondiments(); // 跟对象有关的动作
            if (m_condiments)
                std::cout << m_condiments->description() << "\n";
        }

        void condiments(Condiment *condiments)
        {
            m_condiments = condiments;
        }

    private:
        void boilWater(int amountWaterMl)
        {
            std::cout << "boiling " << amountWaterMl << "ml water\n";
        }

        void pourInCup()
        {
            std::cout << "pour in cup\n";
        }

        Recipe &m_recipe;
        Condiment *m_condiments;

        NO_COPY(CaffeineBeverage);
    };
}

#endif
