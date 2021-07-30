
// g++ -std=c++98 -o strategy strategy.cpp
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

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

namespace classic
{

    namespace beverage // 饮料
    {

        class Receipe // 收据
        {
        public:
            virtual void brew() = 0;          // 继承
            virtual void addCondiments() = 0; // 继承
        };

        class CaffeineBeverage // 咖啡机
        {
        public:
            CaffeineBeverage(Receipe &receipe)
                : m_receipe(receipe)
            {
            }

            void prepareReceipe() // 制作过程
            {
                boilWater();
                m_receipe.brew(); // 跟对象有关的动作
                pourInCup();
                m_receipe.addCondiments(); // 跟对象有关的动作
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

            Receipe &m_receipe;

            NO_COPY(CaffeineBeverage);
        };

        class CoffeeReceipe : public Receipe // 咖啡点单
        {
        public:
            CoffeeReceipe(int minutes)
                : Receipe(), m_minutes(minutes)
            {
            }

            virtual void brew() // 过滤过程
            {
                cout << m_minutes << "min dripping Coffee through filter\n";
            }

            virtual void addCondiments() // 加调味料
            {
                cout << "Adding Sugar and Milk\n";
            }

        private:
            int m_minutes;

            NO_COPY(CoffeeReceipe);
        };

        class TeaReceipe : public Receipe
        {
        public:
            TeaReceipe(int minutes)
                : Receipe(), m_minutes(minutes)
            {
            }

            virtual void brew()
            {
                cout << m_minutes << "min steeping Tea through filter\n";
            }

            virtual void addCondiments()
            {
                cout << "Adding Lemon\n";
            }

        private:
            int m_minutes;

            NO_COPY(TeaReceipe);
        };

        // 调味料基类
        class Condiment
        {
        public:
            Condiment() // 构造函数
                : m_next(0)
            {
            }

            Condiment(Condiment *next) // 复制构造函数
                : m_next(next)
            {
            }

            void add()
            {
                if (m_next)
                    m_next->add();
                this->onAdd();
            }

        private:
            virtual void onAdd() = 0;

            Condiment *m_next;
        };

        //
        class ReceipeWithCondiments : public Receipe
        {
        public:
            virtual void addCondiments()
            {
                m_condiment->add();
            }

        private:
            Condiment *m_condiment;

            NO_COPY(ReceipeWithCondiments);
        };

        class Milk : public Condiment
        {
        public:
            Milk()
                : Condiment()
            {
            }

            Milk(Condiment *next)
                : Condiment(next)
            {
            }

        private:
            virtual void onAdd()
            {
                cout << "-Milk-";
            }

            NO_COPY(Milk);
        };

        class Sugar : public Condiment
        {
        public:
            Sugar()
                : Condiment()
            {
            }

            Sugar(Condiment *next)
                : Condiment(next)
            {
            }

        private:
            virtual void onAdd()
            {
                cout << "-Sugar-";
            }

            NO_COPY(Sugar);
        };

        class Command
        {
        public:
            virtual void execute() = 0;
        };

        class CoffeeMachine
        {
        private:
            typedef vector<Command *> CommandQ;

        public:
            CoffeeMachine()
                : m_commands()
            {
            }

            void request(Command *c)
            {
                m_commands.push_back(c);
            }

            void start()
            {
                for (CommandQ::iterator it(m_commands.begin()); it != m_commands.end(); ++it)
                {
                    (*it)->execute();
                }
            }

        private:
            CommandQ m_commands;

            NO_COPY(CoffeeMachine);
        };

        class MakeCaffeineDrink : public Command
        {
        public:
            MakeCaffeineDrink(CaffeineBeverage &drink)
                : Command(), m_drink(drink)
            {
            }

            virtual void execute()
            {
                m_drink.prepareReceipe();
            }

        private:
            CaffeineBeverage &m_drink;

            NO_COPY(MakeCaffeineDrink);
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

            NO_COPY(MilkFoam);
        };

        class MakeMilkFoam : public Command
        {
        public:
            MakeMilkFoam(MilkFoam &milk, int mlMilk)
                : Command(), m_milk(milk), m_mlMilk(mlMilk)
            {
            }

            virtual void execute()
            {
                m_milk.makeFoam(m_mlMilk);
            }

            void setMlMilk(int mlMilk)
            {
                m_mlMilk = mlMilk;
            }

        private:
            MilkFoam &m_milk;
            int m_mlMilk;

            NO_COPY(MakeMilkFoam);
        };
    }

}

// namespace cpp11
// {

//     namespace beverages
//     {

//         struct Receipe
//         {
//         public:
//             function<void()> brew;
//             function<void()> addCondiments;
//         };

//         class CaffeineBeverage
//         {
//         public:
//             CaffeineBeverage(Receipe receipe)
//             : m_brew(receipe.brew)
//             , m_addCondiments(receipe.addCondiments)
//             {}

//             CaffeineBeverage(function<void()> brew, function<void()> addCondiments)
//             : m_brew(brew)
//             , m_addCondiments(addCondiments)
//             {}

//             void prepareReceipe()
//             {
//                 boilWater();
//                 m_brew();
//                 pourInCup();
//                 m_addCondiments();
//             }

//         private:
//             void boilWater()
//             {
//                 cout << "boil water\n";
//             }

//             void pourInCup()
//             {
//                 cout << "pour in cup\n";
//             }

//             function<void()> m_brew;
//             function<void()> m_addCondiments;

//             NO_COPY_NO_MOVE(CaffeineBeverage);
//         };

//         class Receipes
//         {
//         public:
//             static void brewCoffee(int minutes)
//             {
//                 cout << minutes << "min for dripping Coffee through filter\n";
//             }

//             static void brewTea(int minutes)
//             {
//                 cout << minutes << "min for steeping Tea\n";
//             }

//             static void addSugarAndMilk()
//             {
//                 cout << "Adding Sugar and Milk\n";
//             }

//             static void addLemon()
//             {
//                 cout << "Adding Lemon\n";
//             }

//             NO_COPY_NO_MOVE(Receipes);
//         };

//         class CoffeeMachine
//         {
//         private:
//             typedef vector<function<void()>> CommandQ;

//         public:
//             CoffeeMachine()
//             : m_commands()
//             {}

//             void request(CommandQ::value_type c)
//             {
//                 m_commands.push_back(c);
//             }

//             void start()
//             {
//                 for_each(
//                     begin(m_commands), end(m_commands),
//                     [](CommandQ::value_type c){ c(); });
//             }

//         private:
//             CommandQ m_commands;

//             NO_COPY_NO_MOVE(CoffeeMachine);
//         };

//         class MilkFoam
//         {
//         public:
//             MilkFoam()
//             {}

//             void makeFoam(int mlMilk)
//             {
//                 boilMilk(mlMilk);
//                 pourInCup();
//                 foaming();
//             }

//         private:
//             void boilMilk(int mlMilk)
//             {
//                 cout << "boiling " << mlMilk << "ml milk\n";
//             }

//             void pourInCup()
//             {
//                 cout << "pour in cup\n";
//             }

//             void foaming()
//             {
//                 cout << "foaming\n";
//             }

//             NO_COPY_NO_MOVE(MilkFoam);
//         };

//     }

// }

int main(int argc, char *argv[])
{
    {
        using namespace classic;
        {
            using namespace beverage;
            // 你好👋，我要来一份手冲摩卡咖啡☕️。
            // 好的，开始手冲摩卡咖啡的制作流程。

            // 定时：咖啡粉通过过滤器过滤1分钟
            CoffeeReceipe coffeeReceipe(1);
            // 定时：茶包泡2分钟
            TeaReceipe teaReceipe(2);

            //  初始化：咖啡
            CaffeineBeverage coffee(coffeeReceipe);
            // 初始化：茶
            CaffeineBeverage tea(teaReceipe);

            typedef vector<CaffeineBeverage *> Beverages;
            Beverages beverages;

            // 合并两条订单流程到一个清单
            beverages.push_back(&coffee);
            beverages.push_back(&tea);

            // 然后利用多态机制，开始指定咖啡和茶的流程
            // 烧水->过滤->倒入杯中->加辅料
            for (Beverages::iterator it(beverages.begin()); it != beverages.end(); ++it)
            {
                (*it)->prepareReceipe();
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
            //  coffee在前面已经初始化-->CoffeeReceipe coffeeReceipe(1)
            MakeCaffeineDrink makeCoffee(coffee);

            // 写入流程对像到内存: boilMilk -> pourInCup -> foaming
            MilkFoam milkFoam;
            MakeMilkFoam makeMilkFoam(milkFoam, 100);

            // 清单到位，需要一个咖啡机
            CoffeeMachine coffeeMachine;

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
        }
    }

    return 0;
}
