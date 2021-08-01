#ifndef Command_h
#define Command_h

namespace classic
{
    class Command
    {
    public:
        virtual ~Command() {}
        virtual void execute() = 0;
    };
}
#endif