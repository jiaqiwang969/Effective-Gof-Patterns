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
#include <chrono>

int main(int argc, char *argv[])
{
    //auto start = std::chrono::high_resolution_clock::now();

    using namespace cpp11;
    { // bind
        using Beverages = std::vector<std::unique_ptr<CaffeineBeverage>>;
        Beverages beverages;
        CoffeeMachine coffeeMachine;
        View view;
        BeverageFactory beverageFactory;

        coffeeMachine.getNotifiedOnFinished(std::bind(&View::coffeeMachineFinished, &view));
        do
        {
            std::string inBeverage;
            if (!view.askForBeverage(inBeverage))
                break;
            beverages.emplace_back(beverageFactory.create(inBeverage));
            Condiment condiments;
            do
            {
                CondimentFactory condimentFactory;
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
                coffeeMachine.request(std::bind(&CaffeineBeverage::prepareRecipe, &(*beverage)));
            }
            coffeeMachine.start();
            for (auto &beverage : beverages)
            {
                beverage->description();
                beverage->price();
            }
        }
    }
    { // lambda
        using Beverages = std::vector<std::unique_ptr<CaffeineBeverage>>;
        Beverages beverages;
        CoffeeMachine coffeeMachine;
        View view;
        BeverageFactory beverageFactory;

        coffeeMachine.getNotifiedOnFinished([&]
                                            { view.coffeeMachineFinished(); });
        do
        {
            std::string inBeverage;
            if (!view.askForBeverage(inBeverage))
                break;
            beverages.emplace_back(beverageFactory.create(inBeverage));
            Condiment condiments;
            do
            {
                CondimentFactory condimentFactory;
                std::string inCondiment;
                if (!view.askForCondiments(inCondiment))
                    break;
                Condiment condiment = condimentFactory.create(inCondiment);
                condiments.description = [=]
                { return accu(condiment.description, condiments.description); };
                condiments.price = [=]
                { return accu(condiment.price, condiments.price); };
            } while (true);
            beverages.back()->condiments(condiments);
        } while (true);
        if (!beverages.empty())
        {
            for (auto &beverage : beverages)
            {
                coffeeMachine.request([&]
                                      { beverage->prepareRecipe(); });
            }
            coffeeMachine.start();
            for (auto &beverage : beverages)
            {
                beverage->description();
                beverage->price();
            }
        }
    }
    // auto end = std::chrono::high_resolution_clock::now();
    // auto elapsed = end - start;
    // std::cout << elapsed.count() << '\n';
    return 0;
}
