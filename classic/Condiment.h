#ifndef Condiment_h
#define Condiment_h

#include "../utils/NoCopy.h"
#include <string>

namespace classic
{

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

        std::string description()
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
        virtual std::string onDescription() = 0;
        virtual float onPrice() = 0;

        Condiment *m_next;
    };

}
#endif