
// 想法是用可以将std::function作为参数的函数参数替换抽象类。然后std::function从外部控制多态的灵活性。这样，就可以大大简化许多GoF模式。

// g++ -std=c++11 -o patterns_cpp11 patterns_cpp11.cpp
#include "cpp11/CaffeineBeverage.h"
#include "cpp11/Recipes.h"
#include "cpp11/CoffeeMachine.h"
#include "cpp11/View.h"
#include "cpp11/MilkFoam.h"
#include "cpp11/accu.h"
#include "cpp11/Milk.h"
#include "cpp11/Sugar.h"
#include "cpp11/BeverageFactory.h"

#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <string>
#include <memory>
#include <map>

int main(int argc, char *argv[])
{

    using namespace cpp11;
    {

        CaffeineBeverage coffee(std::bind(&Recipes::brewCoffee, 1), &Recipes::addSugarAndMilk);

        CaffeineBeverage tea(std::bind(&Recipes::brewTea, 2), &Recipes::addLemon);

        typedef std::vector<CaffeineBeverage *> Beverages;
        Beverages beverages;

        beverages.push_back(&coffee);
        beverages.push_back(&tea);

        // for_each(
        //     begin(beverages), end(beverages),
        //     bind(&CaffeineBeverage::prepareRecipe, placeholders::_1));

        for (auto beverage : beverages)
        {
            beverage->prepareRecipe();
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

        View view;
        // coffeeMachine.getNotifiedOnFinished(bind(&View::coffeeMachineFinished, &view));
        coffeeMachine.getNotifiedOnFinished([&]
                                            { view.coffeeMachineFinished(); });

        coffeeMachine.request(std::bind(&CaffeineBeverage::prepareRecipe, &coffee));
        coffeeMachine.request(std::bind(&CaffeineBeverage::prepareRecipe, &tea));

        MilkFoam milkFoam;
        coffeeMachine.request(std::bind(&MilkFoam::makeFoam, &milkFoam, 100));

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
        //Orders are ready to be served

        coffeeMachine.request(std::bind(&MilkFoam::makeFoam, &milkFoam, 200));
        coffeeMachine.request(std::bind(&MilkFoam::makeFoam, &milkFoam, 300));
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
        //Orders are ready to be served

        std::function<std::string()> condimentDescription;
        condimentDescription = bind(&accu<std::string>, &Milk::description, condimentDescription);
        condimentDescription = bind(&accu<std::string>, &Sugar::description, condimentDescription);
        condimentDescription = bind(&accu<std::string>, &Sugar::description, condimentDescription);

        std::function<float()> condimentPrice;
        condimentPrice = bind(&accu<float>, &Milk::price, condimentPrice);
        condimentPrice = bind(&accu<float>, &Sugar::price, condimentPrice);
        condimentPrice = bind(&accu<float>, &Sugar::price, condimentPrice);

        std::cout << "Condiments: " << condimentDescription() << '\n';
        std::cout << "Price: " << condimentPrice() << '\n';
        // results:
        // Condiments : -Sugar-- Sugar-- Milk -
        // Price : 0.07

        BeverageFactory factory;
        factory.create("Coffee")->prepareRecipe();
        factory.create("Tea")->prepareRecipe();
    }
    return 0;
}
