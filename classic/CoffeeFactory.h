#ifndef CoffeeFactory_h
#define CoffeeFactory_h

#include "Coffee.h"
#include "../utils/NoCopy.h"

namespace classic
{

    class CoffeeFactory : public CaffeineBeverageFactory
    {
    public:
        CoffeeFactory()
            : CaffeineBeverageFactory()
        {
        }

        virtual CaffeineBeverage *create()
        {
            return new Coffee();
        }

    private:
        NO_COPY(CoffeeFactory);
    };
}
#endif