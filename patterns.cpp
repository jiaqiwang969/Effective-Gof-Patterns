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
#include "cpp11/Condiment.h"
#include "cpp11/CondimentFactory.h"

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
        std::cout << "---------classic--------" << '\n';

        {
            // 你好👋，我要来一份手冲摩卡咖啡☕️。
            // 好的，开始手冲摩卡咖啡的制作流程。

            std::cout << "---------Strategy--------" << '\n';
            // 定时：咖啡粉通过过滤器过滤1分钟
            CoffeeRecipe coffeeRecipe(151);
            // 定时：茶包泡2分钟
            TeaRecipe teaRecipe(201);

            //  初始化：咖啡
            CaffeineBeverage coffee(coffeeRecipe);
            // 初始化：茶
            CaffeineBeverage tea(teaRecipe);

            typedef std::vector<CaffeineBeverage *> Beverages;
            Beverages beverages;

            // 合并两条订单流程到一个清单
            beverages.push_back(&coffee);
            beverages.push_back(&tea);

            // 然后利用多态机制，开始指定咖啡和茶的流程
            // 烧水->过滤->倒入杯中->加辅料
            for (Beverages::iterator it(beverages.begin()); it != beverages.end(); ++it)
            {
                (*it)->prepareRecipe();
            }

            // 运行得到结果：
            // boil water
            // 1min dripping Coffee through filter
            // pour in cup
            // Adding Sugar and Milk

            // boil water
            // 2min steeping Tea through filter
            // pour in cup
            // Adding Lemon

            std::cout << "---------Command--------" << '\n';

            // 初始化：做的是咖啡-->CaffeineBeverage
            //  coffee在前面已经初始化-->CoffeeRecipe coffeeRecipe(1)
            MakeCaffeineDrink makeCoffee(coffee);
            MakeCaffeineDrink makeTea(tea);

            // 写入流程对像到内存: boilMilk -> pourInCup -> foaming
            MilkFoam milkFoam;
            MakeMilkFoam makeMilkFoam(milkFoam, 100);

            // 清单到位，需要一个咖啡机
            CoffeeMachine coffeeMachine;
            View view;

            coffeeMachine.addObserver(&view);

            // 咖啡机写入清淡
            coffeeMachine.request(new MakeCaffeineDrink(coffee));
            coffeeMachine.request(new MakeCaffeineDrink(tea));
            coffeeMachine.request(new MakeMilkFoam(milkFoam, 100));

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

            std::cout << "----------------" << '\n';

            coffeeMachine.request(new MakeMilkFoam(milkFoam, 200));
            coffeeMachine.request(new MakeMilkFoam(milkFoam, 300));
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

            std::cout << "---------Chain--------" << '\n';

            // 独立的一个chain类
            // Milk milk;
            // Sugar sugarMilk(&milk);
            // Sugar doubleSugarMilk(&sugarMilk);
            Condiment *milk = new Milk();
            Condiment *sugarMilk = new Sugar(milk);
            Condiment *doubleSugarMilk = new Sugar(sugarMilk);

            // std::cout << "Condiments: " << doubleSugarMilk.description() << '\n';
            // std::cout << "Price: " << doubleSugarMilk.price() << '\n';
            std::cout << "Condiments: " << doubleSugarMilk->description() << '\n';
            std::cout << "Price: " << doubleSugarMilk->price() << '\n';

            std::cout << "---------Factory--------" << '\n';

            BeverageFactory factory;
            // factory.create("Coffee")->prepareRecipe();
            // factory.create("Tea")->prepareRecipe();
            CaffeineBeverage *b1 = factory.create("Coffee");
            CaffeineBeverage *b2 = factory.create("Tea");
            b1->prepareRecipe();
            b2->prepareRecipe();
            delete b1;
            delete b2;
        }
    }

    {
        using namespace cpp11;
        std::cout << "---------cpp11--------" << '\n';

        {
            std::cout << "---------cpp11-bind--------" << '\n';

            std::cout << "---------Strategy bind--------" << '\n';

            CaffeineBeverage coffee(std::bind(&Recipes::amountWaterMl, 150), &Recipes::brewCoffee);

            CaffeineBeverage tea(std::bind(&Recipes::amountWaterMl, 200), &Recipes::brewTea);

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

            std::cout << "---------Command bind--------" << '\n';

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

            std::cout << "-----------------" << '\n';

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

            std::cout << "---------Chain bind--------" << '\n';

            Condiment condiments;
            condiments.description = std::bind(&accu<std::string>, &Milk::description, condiments.description);
            condiments.description = std::bind(&accu<std::string>, &Sugar::description, condiments.description);
            condiments.description = std::bind(&accu<std::string>, &Sugar::description, condiments.description);

            condiments.price = bind(&accu<float>, &Milk::price, condiments.price);
            condiments.price = bind(&accu<float>, &Sugar::price, condiments.price);
            condiments.price = bind(&accu<float>, &Sugar::price, condiments.price);

            std::cout << "Condiments: " << condiments.description() << '\n';
            std::cout << "Price: " << condiments.price() << '\n';
            // results:
            // Condiments : -Sugar-- Sugar-- Milk -
            // Price : 0.07

            std::cout << "---------Factory bind--------" << '\n';

            BeverageFactory factory; // 调用构造函数（boost）=> new CaffeineBeverage构造函数(boost::factory封装，bind绑定参数)

            factory.create("Coffee")->prepareRecipe(); // 默认加水3.1
            factory.create("Tea")->prepareRecipe();    // 默认加水4.1
        }

        {
            std::cout << "---------cpp11-lambdas--------" << '\n';
            std::cout << "---------Strategy lambda--------" << '\n';

            CaffeineBeverage coffee([]
                                    { return Recipes::amountWaterMl(150); },
                                    []
                                    { Recipes::brewCoffee(); });
            CaffeineBeverage tea([]
                                 { return Recipes::amountWaterMl(200); },
                                 []
                                 { Recipes::brewTea(); });

            using Beverages = std::vector<CaffeineBeverage *>;
            Beverages beverages;

            beverages.push_back(&coffee);
            beverages.push_back(&tea);

            for (auto beverage : beverages)
            {
                beverage->prepareRecipe();
            }

            std::cout << "---------Command lambda--------" << '\n';
            CoffeeMachine coffeeMachine;
            View view;
            coffeeMachine.getNotifiedOnFinished([&]
                                                { view.coffeeMachineFinished(); });

            MilkFoam milkFoam;
            coffeeMachine.request([&]
                                  { milkFoam.makeFoam(100); });
            coffeeMachine.request([&]
                                  { coffee.prepareRecipe(); });
            coffeeMachine.request([&]
                                  { tea.prepareRecipe(); });
            coffeeMachine.start();

            coffeeMachine.request([&]
                                  { milkFoam.makeFoam(200); });
            coffeeMachine.request([&]
                                  { milkFoam.makeFoam(300); });
            coffeeMachine.start();

            std::cout << "---------Chain lambda--------" << '\n';

            std::function<std::string()> condimentDescription;
            condimentDescription = [=]
            { return accu(&Milk::description, condimentDescription); };
            condimentDescription = [=]
            { return accu(&Sugar::description, condimentDescription); };
            condimentDescription = [=]
            { return accu(&Sugar::description, condimentDescription); };

            std::function<float()> condimentPrice;
            condimentPrice = [=]
            { return accu(&Milk::price, condimentPrice); };
            condimentPrice = [=]
            { return accu(&Sugar::price, condimentPrice); };
            condimentPrice = [=]
            { return accu(&Sugar::price, condimentPrice); };

            std::cout << "Condiments: " << condimentDescription() << '\n';
            std::cout << "Price: " << condimentPrice() << '\n';
        }
    }
    return 0;
}
