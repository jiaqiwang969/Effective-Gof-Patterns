#ifndef cpp11_BeverageFactory_h
#define cpp11_BeverageFactory_h

#include "CaffeineBeverage.h"
#include "Recipes.h"
#include "../utils/NoCopyNoMove.h"

#include <boost/functional/factory.hpp>
#include <functional>
#include <memory>
#include <map>
#include <string>

namespace cpp11
{
    class BeverageFactory
    {
    private:
        typedef std::function<CaffeineBeverage *()> CreateFun;

    public:
        BeverageFactory()
            : m_factory()
        {
            // lambdas method
            // m_factory["Coffee"] =
            //     bind(
            //         boost::factory<CaffeineBeverage *>(),
            //         []
            //         { Recipes::brewCoffee(45); },
            //         []
            //         { Recipes::addSugarAndMilk(); });

            // m_factory["Tea"] =
            //     bind(
            //         boost::factory<CaffeineBeverage *>(),
            //         []
            //         { Recipes::brewTea(37); },
            //         []
            //         { Recipes::addLemon(); });

            m_factory["Coffee"] =
                std::bind(
                    boost::factory<CaffeineBeverage *>(),
                    std::function<int()>(std::bind(&Recipes::amountWaterMl, 4)), // 默认加水量
                    &Recipes::brewCoffee);

            m_factory["Tea"] =
                std::bind(
                    boost::factory<CaffeineBeverage *>(),
                    std::function<int()>(std::bind(&Recipes::amountWaterMl, 5)), // 默认加水量
                    &Recipes::brewTea);
        }

        // CaffeineBeverage *create(string const &beverage)
        // {
        //     return m_factory[beverage]();
        // }
        std::unique_ptr<CaffeineBeverage> create(std::string const &beverage)
        {
            return std::unique_ptr<CaffeineBeverage>(m_factory[beverage]());
        }

    private:
        std::map<std::string, CreateFun> m_factory;

        NO_COPY_NO_MOVE(BeverageFactory);
    };
}

#endif