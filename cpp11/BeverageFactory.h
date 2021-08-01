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

            // m_factory["Coffee"] =
            //     std::bind(
            //         boost::factory<CaffeineBeverage *>(),
            //         std::function<int()>(std::bind(&Recipes::amountWaterMl, 3)),
            //         &Recipes::brewCoffee);

            // m_factory["Tea"] =
            //     std::bind(
            //         boost::factory<CaffeineBeverage *>(),
            //         std::function<int()>(std::bind(&Recipes::amountWaterMl, 4)),
            //         &Recipes::brewTea);

            m_factory["Coffee"] = []
            {
                return new CaffeineBeverage(
                    []
                    { return 3; },
                    &Recipes::brewCoffee, "Coffee", 2.48f);
            };

            m_factory["Tea"] = []
            {
                return new CaffeineBeverage(
                    []
                    { return 4; },
                    &Recipes::brewTea, "Tea", 1.27f);
            };
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