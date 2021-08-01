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
            : m_recipe(recipe), m_condiments(0), m_description("CaffeineBeverage"), m_price(0.0f)
        {
        }

        CaffeineBeverage(Recipe &receipe, std::string description, float price)
            : m_recipe(receipe), m_condiments(0), m_description(description), m_price(price) {}

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

        void description()
        {
            std::cout << m_description;
            if (m_condiments)
            {
                std::cout << " : " << m_condiments->description();
            }
            std::cout << '\n';
        }

        void price()
        {
            if (m_condiments)
            {
                std::cout << m_price + m_condiments->price() << '\n';
            }
            else
            {
                std::cout << m_price << '\n';
            }
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
        std::string m_description;
        float m_price;

        NO_COPY(CaffeineBeverage);
    };
}

#endif
