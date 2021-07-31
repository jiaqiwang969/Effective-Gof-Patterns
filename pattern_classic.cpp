//Original UML ---> https : //cdn.mathpix.com/snip/images/RB2SQ9jYlXyQAKT8eWtOSsPHqL_pKrZiR4sQPlYa4KY.original.fullsize.png

// g++ -std=c++98 -o pattern_classic pattern_classic.cpp

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

#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <string>
#include <map>

int main(int argc, char *argv[])
{
    {
        using namespace classic;
        {
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

            // ------------------------------------

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

            // 独立的一个chain类
            Milk milk;
            Sugar sugarMilk(&milk);
            Sugar doubleSugarMilk(&sugarMilk);

            std::cout << "Condiments: " << doubleSugarMilk.description() << '\n';
            std::cout << "Price: " << doubleSugarMilk.price() << '\n';

            BeverageFactory factory;
            factory.create("Coffee")->prepareRecipe();
            factory.create("Tea")->prepareRecipe();
        }
    }

    return 0;
}
