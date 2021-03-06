// g++ -std=c++98 -o App App.cpp

#include "CoffeeRecipe.h"
#include "TeaRecipe.h"
#include "CaffeineBeverage.h"
#include "MakeCaffeineDrink.h"
#include "MilkFoam.h"
#include "MakeMilkFoam.h"
#include "CoffeeMachine.h"
#include "View.h"
#include "Milk.h"
#include "Sugar.h"
#include "BeverageFactory.h"
#include "CondimentFactory.h"

int main(int argc, char *argv[])
{
    using namespace classic;

    typedef std::vector<CaffeineBeverage *> Beverages;
    Beverages beverages;
    CoffeeMachine coffeeMachine;
    View view;
    BeverageFactory beverageFactory;

    coffeeMachine.addObserver(&view);
    do
    {
        std::string inBeverage;
        if (!view.askForBeverage(inBeverage))
            break;
        beverages.push_back(beverageFactory.create(inBeverage));
        CondimentFactory condimentFactory;
        Condiment *condiments = 0;
        do
        {
            std::string inCondiment;
            if (!view.askForCondiments(inCondiment))
                break;
            condiments = condimentFactory.create(inCondiment, condiments);
        } while (true);
        beverages.back()->condiments(condiments);
    } while (true);
    if (!beverages.empty())
    {
        for (Beverages::iterator it(beverages.begin()); it != beverages.end(); ++it)
        {
            coffeeMachine.request(new MakeCaffeineDrink(**it));
        }
        coffeeMachine.start();
        do
        {
            beverages.back()->description();
            beverages.back()->price();
            delete beverages.back();
            beverages.pop_back();
        } while (!beverages.empty());
    }

    return 0;
}
