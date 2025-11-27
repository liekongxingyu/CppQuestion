#include <iostream>

class Complex
{
public:
    double real;
    double imag;

    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    // const成员函数意为这个函数不会修改它的成员变量
    Complex operator+(const Complex &other) const
    {
        return Complex(real + other.real, imag + other.imag);
    }
};

int main()
{
    double a, b, c, d;
    std::cin >> a >> b;
    std::cin >> c >> d;

    Complex x(a, b), y(c, d);
    Complex sum = x + y;

    std::cout << sum.real << "+" << sum.imag << "i";

    return 0;
}
