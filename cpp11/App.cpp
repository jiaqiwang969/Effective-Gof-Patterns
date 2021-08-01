// g++ -std=c++11 -o App App.cpp

#include "CaffeineBeverage.h"
#include "Recipes.h"
#include "CoffeeMachine.h"
#include "View.h"
#include "MilkFoam.h"
#include "accu.h"
#include "Milk.h"
#include "Sugar.h"
#include "BeverageFactory.h"
#include "Condiment.h"
#include "CondimentFactory.h"

#include <functional>
#include <vector>
#include <memory>

int main(int argc, char *argv[])
{
    using namespace cpp11;

    std::cout << "---------CoffeeMachine-App-Cpp11--------" << '\n';

    CoffeeMachine coffeeMachine;
    View view;
    coffeeMachine.getNotifiedOnFinished(std::bind(&View::coffeeMachineFinished, &view));

    typedef std::vector<std::unique_ptr<CaffeineBeverage>> Beverages;
    Beverages beverages;

    BeverageFactory beverageFactory;
    CondimentFactory condimentFactory;
    Condiment condiments;

    do
    {
        std::string inBeverage;
        if (!view.askForBeverage(inBeverage))
            break;
        beverages.emplace_back(beverageFactory.create(inBeverage));
        std::cout << "Choose condiments or q for next beverage order:" << std::endl;

        do
        {
            std::string inCondiment;
            if (!view.askForCondiments(inCondiment))
                break;
            Condiment condiment = condimentFactory.create(inCondiment);
            condiments.description = std::bind(&accu<std::string>, condiment.description, condiments.description);
            condiments.price = std::bind(&accu<float>, condiment.price, condiments.price);
        } while (true);

        beverages.back()->condiments(condiments);

    } while (true);
    if (!beverages.empty())
    {
        for (auto &beverage : beverages)
        {
            coffeeMachine.request(std::bind(&CaffeineBeverage::prepareRecipe, beverage.get()));
        }
        coffeeMachine.start();
    }
    else
    {
    }
}