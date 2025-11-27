#include <iostream>

class Box
{
private:
    double h;
    double w;
    double c;

    friend double volume(const Box &b);

public:
    Box(double h, double w, double c) : h(h), w(w), c(c) {}
};

// 传const是因为只读不写
double volume(const Box &b)
{
    return b.h * b.w * b.c;
}

int main()
{
    Box b(1, 2, 3);
    std::cout << volume(b);

    return 0;
}
