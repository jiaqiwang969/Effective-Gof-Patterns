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
    std::cout << "---------CoffeeMachine-App-classic--------" << '\n';

    using namespace classic;

    CoffeeMachine coffeeMachine;
    View view;
    coffeeMachine.addObserver(&view);

    MilkFoam milkFoam;
    MakeMilkFoam makeMilkFoam(milkFoam, 101);

    typedef std::vector<MakeCaffeineDrink *> MakeCaffeineDrinks;
    MakeCaffeineDrinks makeCaffeineDrinks;

    typedef std::vector<CaffeineBeverage *> Beverages;
    Beverages beverages;

    BeverageFactory beverageFactory;
    CondimentFactory condimentFactory;
    Condiment *condiments = 0;

    do //request
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
            makeCaffeineDrinks.push_back(new MakeCaffeineDrink(**it));
            coffeeMachine.request(makeCaffeineDrinks.back());
            coffeeMachine.request(&makeMilkFoam);
        }
        coffeeMachine.start();
        do
        {
            delete beverages.back();
            beverages.pop_back();
        } while (!beverages.empty());
    }
    else
    {
    }
}