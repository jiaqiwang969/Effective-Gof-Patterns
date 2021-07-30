
// 想法是用可以将std::function作为参数的函数参数替换抽象类。然后std::function从外部控制多态的灵活性。这样，就可以大大简化许多GoF模式。

// g++ -std=c++11 -o strategy_11 strategy_11.cpp
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <string>
#include <boost/signals2.hpp>

#define NO_COPY(className)        \
private:                          \
    className(className const &); \
                                  \
private:                          \
    className &operator=(className const &)

#define NO_MOVE(className)   \
private:                     \
    className(className &&); \
                             \
private:                     \
    className &&operator=(className &&)

#define NO_COPY_NO_MOVE(className) \
    NO_COPY(className);            \
    NO_MOVE(className)

using namespace std;

namespace cpp11
{

    namespace beverages
    {

        struct Receipe
        {
        public:
            function<void()> brew;
            function<void()> addCondiments;
        };

        class CaffeineBeverage
        {
        public:
            //  构造函数1
            CaffeineBeverage(Receipe receipe)
                : m_brew(receipe.brew), m_addCondiments(receipe.addCondiments)
            {
            }

            // 构造函数2
            CaffeineBeverage(function<void()> brew, function<void()> addCondiments)
                : m_brew(brew), m_addCondiments(addCondiments)
            {
            }

            void prepareReceipe()
            {
                boilWater();
                m_brew();
                pourInCup();
                m_addCondiments();
            }

        private:
            void boilWater()
            {
                cout << "boil water\n";
            }

            void pourInCup()
            {
                cout << "pour in cup\n";
            }

            function<void()> m_brew;
            function<void()> m_addCondiments;

            NO_COPY_NO_MOVE(CaffeineBeverage);
        };

        class Receipes
        {
        public:
            static void brewCoffee(int minutes)
            {
                cout << minutes << "min for dripping Coffee through filter\n";
            }

            static void brewTea(int minutes)
            {
                cout << minutes << "min for steeping Tea\n";
            }

            static void addSugarAndMilk()
            {
                cout << "Adding Sugar and Milk\n";
            }

            static void addLemon()
            {
                cout << "Adding Lemon\n";
            }

            NO_COPY_NO_MOVE(Receipes);
        };

        class CoffeeMachine
        {
        private:
            typedef vector<function<void()>> CommandQ;

        public:
            CoffeeMachine()
                : m_commands()
            {
            }

            void request(CommandQ::value_type c)
            {
                m_commands.push_back(c);
            }

            void start()
            {
                for_each(
                    begin(m_commands), end(m_commands),
                    [](CommandQ::value_type c)
                    { c(); });
            }

        private:
            CommandQ m_commands;

            NO_COPY_NO_MOVE(CoffeeMachine);
        };

        class MilkFoam
        {
        public:
            MilkFoam()
            {
            }

            void makeFoam(int mlMilk)
            {
                boilMilk(mlMilk);
                pourInCup();
                foaming();
            }

        private:
            void boilMilk(int mlMilk)
            {
                cout << "boiling " << mlMilk << "ml milk\n";
            }

            void pourInCup()
            {
                cout << "pour in cup\n";
            }

            void foaming()
            {
                cout << "foaming\n";
            }

            NO_COPY_NO_MOVE(MilkFoam);
        };

        struct Condiment
        {
            function<string()> description;
            function<float()> price;
        };

        template <typename Res>
        static Res accu(function<Res()> call, function<Res()> next)
        {
            if (next)
                return call() + next();
            return call();
        }

        class Milk
        {
        public:
            static string description()
            {
                return "-Milk-";
            }

            static float price()
            {
                return 0.13f;
            }
        };

        class Sugar
        {
        public:
            static string description()
            {
                return "-Sugar-";
            }

            static float price()
            {
                return 0.07f;
            }
        };

    }
}

int main(int argc, char *argv[])
{

    {
        using namespace cpp11;
        {
            using namespace beverages;

            CaffeineBeverage coffee(bind(&Receipes::brewCoffee, 1), &Receipes::addSugarAndMilk);

            CaffeineBeverage tea(bind(&Receipes::brewTea, 2), &Receipes::addLemon);

            typedef vector<CaffeineBeverage *> Beverages;
            Beverages beverages;

            beverages.push_back(&coffee);
            beverages.push_back(&tea);

            using namespace placeholders;

            // for_each(
            //     begin(beverages), end(beverages),
            //     bind(&CaffeineBeverage::prepareReceipe, _1));
            for (auto beverage : beverages)
            {
                beverage->prepareReceipe();
            }

            // result:
            // boil water
            // 1min for dripping Coffee through filter
            // pour in cup
            // Adding Sugar and Milk
            // boil water
            // 2min for steeping Tea
            // pour in cup
            // Adding Lemon

            CoffeeMachine coffeeMachine;

            coffeeMachine.request(bind(&CaffeineBeverage::prepareReceipe, &coffee));
            coffeeMachine.request(bind(&CaffeineBeverage::prepareReceipe, &tea));

            MilkFoam milkFoam;
            coffeeMachine.request(bind(&MilkFoam::makeFoam, &milkFoam, 100));

            coffeeMachine.start();
            // result:
            // boil water
            // 1min for dripping Coffee through filter
            // pour in cup
            // Adding Sugar and Milk
            // boil water
            // 2min for steeping Tea
            // pour in cup
            // Adding Lemon
            // boiling 100ml milk
            // pour in cup
            // foaming

            coffeeMachine.request(bind(&MilkFoam::makeFoam, &milkFoam, 200));
            coffeeMachine.request(bind(&MilkFoam::makeFoam, &milkFoam, 300));
            coffeeMachine.start();
            // results:
            // boil water
            // 1min for dripping Coffee through filter
            // pour in cup
            // Adding Sugar and Milk
            // boil water
            // 2min for steeping Tea
            // pour in cup
            // Adding Lemon
            // boiling 100ml milk
            // pour in cup
            // foaming
            // boiling 200ml milk
            // pour in cup
            // foaming
            // boiling 300ml milk
            // pour in cup
            // foaming

            Condiment condiments;
            // condiments.description = bind(&accu<string>, &Milk::description, condiments.description);
            // condiments.description = bind(&accu<string>, &Sugar::description, condiments.description);
            // condiments.description = bind(&accu<string>, &Sugar::description, condiments.description);
            condiments.description = [=]
            { return accu<string>(&Milk::description, condiments.description); };
            condiments.description = [=]
            { return accu<string>(&Sugar::description, condiments.description); };
            condiments.description = [=]
            { return accu<string>(&Sugar::description, condiments.description); };

            function<float()> condimentPrice;
            // condiments.price = bind(&accu<float>, &Milk::price, condimentPrice);
            // condiments.price = bind(&accu<float>, &Sugar::price, condimentPrice);
            // condiments.price = bind(&accu<float>, &Sugar::price, condimentPrice);
            condiments.price = [=]
            { return accu<float>(&Milk::price, condimentPrice); };
            condiments.price = [=]
            { return accu<float>(&Milk::price, condimentPrice); };
            condiments.price = [=]
            { return accu<float>(&Milk::price, condimentPrice); };

            cout << "Condiments: " << condiments.description() << '\n';
            cout << "Price: " << condiments.price() << '\n';
            // results:
            // Condiments : -Sugar-- Sugar-- Milk -
            // Price : 0.07
        }
    }
    return 0;
}
