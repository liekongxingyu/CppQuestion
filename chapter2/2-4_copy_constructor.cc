#include <iostream>

class MyNumber
{
private:
    const int value;

public:
    MyNumber(int v) : value(v)
    {
        std::cout << "Ctor" << std::endl;
    }

    MyNumber(const MyNumber &other) : value(other.value)
    {
        std::cout << "CopyCtor" << std::endl;
    }

    ~MyNumber()
    {
        std::cout << "out" << std::endl;
    }
};

int main()
{
    MyNumber a(5);
    MyNumber b = a;

    return 0;
}
