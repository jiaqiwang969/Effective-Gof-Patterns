#ifndef TeaRecipe_h
#define TeaRecipe_h

#include "../utils/NoCopy.h"
#include <iostream>

namespace classic
{
    class TeaRecipe : public Recipe
    {
    public:
        TeaRecipe(int minutes)
            : Recipe(), m_minutes(minutes)
        {
        }

        virtual void brew()
        {
            std::cout << m_minutes << "min steeping Tea through filter\n";
        }

        virtual void addCondiments()
        {
            std::cout << "Adding Lemon\n";
        }

    private:
        int m_minutes;

        NO_COPY(TeaRecipe);
    };
}
#endif