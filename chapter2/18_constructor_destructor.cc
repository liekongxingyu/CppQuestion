#include <iostream>

class Logger
{
public:
    Logger()
    {
        std::cout << "Constructed" << std::endl;
    }

    ~Logger()
    {
        std::cout << "Destructed" << std::endl;
    }
};

int main()
{
    Logger log;

    return 0;
}
