#ifndef Tea_h
#define Tea_h

#include "../utils/NoCopy.h"

namespace classic
{
    class Tea : public CaffeineBeverage
    {
    public:
        Tea()
            : CaffeineBeverage(m_Recipe), m_Recipe(4) // 默认加水量
        {
        }

    private:
        TeaRecipe m_Recipe;
        NO_COPY(Tea);
    };
}
#endif