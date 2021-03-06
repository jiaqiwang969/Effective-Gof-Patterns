#ifndef MakeMilkFoam_h
#define MakeMilkFoam_h

#include "../utils/NoCopy.h"

namespace classic
{

    class MakeMilkFoam : public Command
    {
    public:
        MakeMilkFoam(MilkFoam &milk, int mlMilk)
            : Command(), m_milk(milk), m_mlMilk(mlMilk)
        {
        }

        virtual void execute()
        {
            m_milk.makeFoam(m_mlMilk);
        }

        void amountML(int ml)
        {
            m_mlMilk = ml;
        }

    private:
        MilkFoam &m_milk;
        int m_mlMilk;

        NO_COPY(MakeMilkFoam);
    };

}
#endif