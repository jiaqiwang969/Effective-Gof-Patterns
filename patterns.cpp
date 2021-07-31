//g++ -std=c++11 -o patterns patterns.cpp

#include "classic/CoffeeRecipe.h"
#include "classic/TeaRecipe.h"
#include "classic/CaffeineBeverage.h"
#include "classic/MakeCaffeineDrink.h"
#include "classic/MilkFoam.h"
#include "classic/MakeMilkFoam.h"
#include "classic/CoffeeMachine.h"
#include "classic/View.h"
#include "classic/Milk.h"
#include "classic/Sugar.h"
#include "classic/BeverageFactory.h"
#include "classic/CondimentFactory.h"

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
    {
        using namespace classic;
        {
            std::cout << "---------classic--------" << '\n';
            // 你好👋，我要来一份手冲摩卡咖啡☕️。
            // 好的，开始手冲摩卡咖啡的制作流程。

            // 定时：咖啡粉通过过滤器过滤1分钟
            CoffeeRecipe coffeeRecipe(1);
            // 定时：茶包泡2分钟
            TeaRecipe teaRecipe(2);

            //  初始化：咖啡
            CaffeineBeverage coffee(coffeeRecipe);
            // 初始化：茶
            CaffeineBeverage tea(teaRecipe);

            typedef std::vector<CaffeineBeverage *> Beverages;
            Beverages beverages;

            // // 合并两条订单流程到一个清单
            // beverages.push_back(&coffee);
            // beverages.push_back(&tea);

            // // 然后利用多态机制，开始指定咖啡和茶的流程
            // // 烧水->过滤->倒入杯中->加辅料
            // for (Beverages::iterator it(beverages.begin()); it != beverages.end(); ++it)
            // {
            //     (*it)->prepareRecipe();
            // }

            // 运行得到结果：
            // boil water
            // 1min dripping Coffee through filter
            // pour in cup
            // Adding Sugar and Milk

            // boil water
            // 2min steeping Tea through filter
            // pour in cup
            // Adding Lemon

            std::cout << "---------分割线--------" << '\n';

            // 初始化：做的是咖啡-->CaffeineBeverage
            //  coffee在前面已经初始化-->CoffeeRecipe coffeeRecipe(1)
            MakeCaffeineDrink makeCoffee(coffee);

            // 写入流程对像到内存: boilMilk -> pourInCup -> foaming
            MilkFoam milkFoam;
            MakeMilkFoam makeMilkFoam(milkFoam, 100);

            // 清单到位，需要一个咖啡机
            CoffeeMachine coffeeMachine;
            View view;

            coffeeMachine.addObserver(&view);

            // 咖啡机写入清淡
            coffeeMachine.request(&makeCoffee);
            coffeeMachine.request(&makeMilkFoam);

            // 启动 (*it)->execute();
            // 策略模式，按照给定的流程依次执行
            coffeeMachine.start();

            // 输出结果
            // boil water
            // 1min dripping Coffee through filter
            // pour in cup
            // Adding Sugar and Milk

            // boiling 100ml milk
            // pour in cup
            // foaming

            // 重新写入新的值

            std::cout << "---------分割线--------" << '\n';

            makeMilkFoam.setMlMilk(200);
            // request -->push_back 到执行队列
            coffeeMachine.request(&makeMilkFoam);
            makeMilkFoam.setMlMilk(300);
            // request -->push_back 到执行队列
            coffeeMachine.request(&makeMilkFoam);
            // 执行
            coffeeMachine.start();

            // boil water
            // 1min dripping Coffee through filter
            // pour in cup
            // Adding Sugar and Milk
            // boiling 300ml milk
            // pour in cup
            // foaming
            // boiling 300ml milk
            // pour in cup
            // foaming
            // boiling 300ml milk
            // pour in cup
            // foaming

            std::cout << "---------分割线--------" << '\n';

            // 独立的一个chain类
            Milk milk;
            Sugar sugarMilk(&milk);
            Sugar doubleSugarMilk(&sugarMilk);

            std::cout << "Condiments: " << doubleSugarMilk.description() << '\n';
            std::cout << "Price: " << doubleSugarMilk.price() << '\n';

            BeverageFactory factory;
            factory.create("Coffee")->prepareRecipe();
            factory.create("Tea")->prepareRecipe();

            std::cout << "---------分割线-测试--------" << '\n';
            {
                typedef std::vector<CaffeineBeverage *> Beverages;
                Beverages beverages;
                CoffeeMachine coffeeMachine;
                View view;
                BeverageFactory beverageFactory;

                coffeeMachine.addObserver(&view);
                do
                {
                    std::cout << "Coffeemachine now ready for taking orders or q for quit!" << std::endl;
                    std::string inBeverage;
                    std::getline(std::cin, inBeverage);
                    if (inBeverage == "q")
                        break;
                    beverages.push_back(beverageFactory.create(inBeverage));
                    std::cout << "Choose condiments or q for next beverage order:" << std::endl;
                    std::string inCondiment;
                    CondimentFactory condimentFactory;
                    Condiment *condiments = 0;
                    do
                    {
                        std::getline(std::cin, inCondiment);
                        if (inCondiment == "q")
                            break;
                        condiments = condimentFactory.create(inCondiment, condiments);
                    } while (true);
                    beverages.back()->condiments(condiments);
                } while (true);
                if (!beverages.empty())
                {
                    typedef std::vector<MakeCaffeineDrink *> MakeCaffeineDrinks;
                    MakeCaffeineDrinks makeCaffeineDrinks;
                    for (Beverages::iterator it(beverages.begin()); it != beverages.end(); ++it)
                    {
                        makeCaffeineDrinks.push_back(new MakeCaffeineDrink(**it));
                        coffeeMachine.request(makeCaffeineDrinks.back());
                    }
                    coffeeMachine.start();
                    do
                    {
                        delete beverages.back();
                        beverages.pop_back();
                    } while (!beverages.empty());
                    do
                    {
                        delete makeCaffeineDrinks.back();
                        makeCaffeineDrinks.pop_back();
                    } while (!makeCaffeineDrinks.empty());
                }
                else
                {
                }

                std::cout << "---------分割线--------" << '\n';

                // {
                //     typedef std::vector<CaffeineBeverage *> Beverages;
                //     Beverages beverages;
                //     CoffeeMachine coffeeMachine;
                //     View view;

                //     coffeeMachine.addObserver(&view);
                //     do
                //     {
                //         std::cout << "Coffeemachine now ready for taking orders or q for quit!" << std::endl;
                //         std::string inBeverage;
                //         std::getline(std::cin, inBeverage);
                //         if (inBeverage == "q")
                //             break;
                //         beverages.push_back(factory.create(inBeverage));
                //         std::cout << "Choose condiments or q for next beverage order:" << std::endl;
                //         std::string inCondiment;
                //         CondimentFactory condimentFactory;
                //         Condiment *condiments = 0;
                //         do
                //         {
                //             std::getline(std::cin, inCondiment);
                //             if (inCondiment == "q")
                //                 break;
                //             condiments = condimentFactory.create(inCondiment, condiments);
                //         } while (true);
                //         beverages.back()->condiments(condiments);
                //     } while (true);
                //     if (!beverages.empty())
                //     {
                //         typedef std::vector<MakeCaffeineDrink *> MakeCaffeineDrinks;
                //         MakeCaffeineDrinks makeCaffeineDrinks;
                //         for (Beverages::iterator it(beverages.begin()); it != beverages.end(); ++it)
                //         {
                //             makeCaffeineDrinks.push_back(new MakeCaffeineDrink(**it));
                //             coffeeMachine.request(makeCaffeineDrinks.back());
                //         }
                //         coffeeMachine.start();
                //         do
                //         {
                //             delete beverages.back();
                //             beverages.pop_back();
                //         } while (!beverages.empty());
                //         do
                //         {
                //             delete makeCaffeineDrinks.back();
                //             makeCaffeineDrinks.pop_back();
                //         } while (!makeCaffeineDrinks.empty());
                //     }
                //     else
                //     {
                //     }
                // }
            }
        }
    }

    // {
    //     using namespace cpp11;
    //     {
    //         std::cout << "---------cpp11--------" << '\n';

    //         CaffeineBeverage coffee(std::bind(&Recipes::brewCoffee, 1), &Recipes::addSugarAndMilk);

    //         CaffeineBeverage tea(std::bind(&Recipes::brewTea, 2), &Recipes::addLemon);

    //         typedef std::vector<CaffeineBeverage *> Beverages;
    //         Beverages beverages;

    //         beverages.push_back(&coffee);
    //         beverages.push_back(&tea);

    //         // for_each(
    //         //     begin(beverages), end(beverages),
    //         //     bind(&CaffeineBeverage::prepareRecipe, placeholders::_1));

    //         for (auto beverage : beverages)
    //         {
    //             beverage->prepareRecipe();
    //         }

    //         // result:
    //         // boil water
    //         // 1min for dripping Coffee through filter
    //         // pour in cup
    //         // Adding Sugar and Milk
    //         // boil water
    //         // 2min for steeping Tea
    //         // pour in cup
    //         // Adding Lemon

    //         std::cout << "---------分割线--------" << '\n';

    //         CoffeeMachine coffeeMachine;

    //         View view;
    //         // coffeeMachine.getNotifiedOnFinished(bind(&View::coffeeMachineFinished, &view));
    //         coffeeMachine.getNotifiedOnFinished([&]
    //                                             { view.coffeeMachineFinished(); });

    //         coffeeMachine.request(std::bind(&CaffeineBeverage::prepareRecipe, &coffee));
    //         coffeeMachine.request(std::bind(&CaffeineBeverage::prepareRecipe, &tea));

    //         MilkFoam milkFoam;
    //         coffeeMachine.request(std::bind(&MilkFoam::makeFoam, &milkFoam, 100));

    //         coffeeMachine.start();
    //         // result:
    //         // boil water
    //         // 1min for dripping Coffee through filter
    //         // pour in cup
    //         // Adding Sugar and Milk
    //         // boil water
    //         // 2min for steeping Tea
    //         // pour in cup
    //         // Adding Lemon
    //         // boiling 100ml milk
    //         // pour in cup
    //         // foaming
    //         //Orders are ready to be served

    //         std::cout << "---------分割线--------" << '\n';

    //         coffeeMachine.request(std::bind(&MilkFoam::makeFoam, &milkFoam, 200));
    //         coffeeMachine.request(std::bind(&MilkFoam::makeFoam, &milkFoam, 300));
    //         coffeeMachine.start();
    //         // results:
    //         // boil water
    //         // 1min for dripping Coffee through filter
    //         // pour in cup
    //         // Adding Sugar and Milk
    //         // boil water
    //         // 2min for steeping Tea
    //         // pour in cup
    //         // Adding Lemon
    //         // boiling 100ml milk
    //         // pour in cup
    //         // foaming
    //         // boiling 200ml milk
    //         // pour in cup
    //         // foaming
    //         // boiling 300ml milk
    //         // pour in cup
    //         // foaming
    //         //Orders are ready to be served

    //         std::cout << "---------分割线--------" << '\n';

    //         std::function<std::string()> condimentDescription;
    //         condimentDescription = bind(&accu<std::string>, &Milk::description, condimentDescription);
    //         condimentDescription = bind(&accu<std::string>, &Sugar::description, condimentDescription);
    //         condimentDescription = bind(&accu<std::string>, &Sugar::description, condimentDescription);

    //         std::function<float()> condimentPrice;
    //         condimentPrice = bind(&accu<float>, &Milk::price, condimentPrice);
    //         condimentPrice = bind(&accu<float>, &Sugar::price, condimentPrice);
    //         condimentPrice = bind(&accu<float>, &Sugar::price, condimentPrice);

    //         std::cout << "Condiments: " << condimentDescription() << '\n';
    //         std::cout << "Price: " << condimentPrice() << '\n';
    //         // results:
    //         // Condiments : -Sugar-- Sugar-- Milk -
    //         // Price : 0.07

    //         std::cout << "---------分割线--------" << '\n';

    //         BeverageFactory factory;
    //         factory.create("Coffee")->prepareRecipe();
    //         factory.create("Tea")->prepareRecipe();
    //     }
    // }

    return 0;
}
