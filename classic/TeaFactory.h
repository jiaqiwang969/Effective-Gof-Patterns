#ifndef TeaFactory_h
#define TeaFactory_h

#include "Tea.h"
#include "../utils/NoCopy.h"

namespace classic
{
    class TeaFactory : public CaffeineBeverageFactory
    {
    public:
        TeaFactory()
            : CaffeineBeverageFactory()
        {
        }

        virtual CaffeineBeverage *create()
        {
            return new Tea();
        }

    private:
        NO_COPY(TeaFactory);
    };
}
#endif