#ifndef cpp11_Recipes_h
#define cpp11_Recipes_h

#include <iostream>
#include "../utils/NoCopyNoMove.h"

namespace cpp11
{
    class Recipes
    {
    public:
        static void brewCoffee(int minutes)
        {
            std::cout << minutes << "min for dripping Coffee through filter\n";
        }

        static void brewTea(int minutes)
        {
            std::cout << minutes << "min for steeping Tea\n";
        }

        static void addSugarAndMilk()
        {
            std::cout << "Adding Sugar/Milk\n";
        }

        static void addLemon()
        {
            std::cout << "Adding Lemon\n";
        }

        NO_COPY_NO_MOVE(Recipes);
    };

}

#endif