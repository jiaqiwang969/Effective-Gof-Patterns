//Original UML ---> https : //cdn.mathpix.com/snip/images/RB2SQ9jYlXyQAKT8eWtOSsPHqL_pKrZiR4sQPlYa4KY.original.fullsize.png

// g++ -std=c++98 -o strategy strategy.cpp
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <string>

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

    namespace beverages // 饮料
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

            string description()
            {
                if (m_next)
                    return this->onDescription() + m_next->description();
                return this->onDescription();
            }
            float price()
            {
                if (m_next)
                    return this->onPrice() + m_next->price();
                return this->onPrice();
            }

        private:
            virtual string onDescription() = 0;
            virtual float onPrice() = 0;

            Condiment *m_next;
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
            virtual string onDescription()
            {
                return "-Milk-";
            }

            virtual float onPrice()
            {
                return 0.13f;
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
            virtual string onDescription()
            {
                return "-Sugar-";
            }

            virtual float onPrice()
            {
                return 0.07f;
            }

            NO_COPY(Sugar);
        };

        class Command
        {
        public:
            virtual void execute() = 0;
        };

        class CoffeeMachine;

        class CoffeeMachineState
        {
        public:
            virtual void action(CoffeeMachine &coffeeMachine, string const &cmd) = 0;
        };

        class CoffeeMachineObserver
        {
        public:
            virtual void finished() = 0;
        };

        class View : public CoffeeMachineObserver
        {
        public:
            View()
                : CoffeeMachineObserver()
            {
            }

            virtual void finished()
            {
                cout << "Orders are ready to be served\n";
            }

        private:
            NO_COPY(View);
        };

        class ObservableCoffeeMachine
        {
        private:
            typedef vector<CoffeeMachineObserver *> Observers;

        public:
            ObservableCoffeeMachine()
                : m_observers()
            {
            }

            void addObserver(Observers::value_type o)
            {
                m_observers.push_back(o);
            }

            void removeObserver(Observers::value_type o)
            {
                Observers::iterator it = find(m_observers.begin(), m_observers.end(), o);
                if (it != m_observers.end())
                    m_observers.erase(it);
            }

        protected:
            void notifyFinished()
            {
                for (Observers::iterator it(m_observers.begin()); it != m_observers.end(); ++it)
                {
                    (*it)->finished();
                }
            }

        private:
            Observers m_observers;

            NO_COPY(ObservableCoffeeMachine);
        };

        class CoffeeMachine : public ObservableCoffeeMachine
        {
        private:
            typedef vector<Command *> CommandQ;

        public:
            CoffeeMachine()
                : ObservableCoffeeMachine(), m_commands(), m_state(0)
            {
            }

            void setState(CoffeeMachineState *newState)
            {
                m_state = newState;
            }

            void execute(string const &cmd)
            {
                m_state->action(*this, cmd);
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
                this->notifyFinished();
            }

        private:
            CommandQ m_commands;
            CoffeeMachineState *m_state;

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

int main(int argc, char *argv[])
{
    {
        using namespace classic;
        {
            using namespace beverages;
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

            cout << "Condiments: " << doubleSugarMilk.description() << '\n';
            cout << "Price: " << doubleSugarMilk.price() << '\n';
        }
    }

    return 0;
}
