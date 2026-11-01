#include <iostream>

class Point
{
public:
    int x;
    int y;

    Point(int x, int y) : x(x), y(y)
    {
    }

    // const成员函数的用法
    void print() const
    {
        std::cout << "x: " << x << " " << "y: " << y << std::endl;
    }
};

int main()
{
    Point p(2, 3);
    p.print();
    return 0;
}
