#include <iostream>
using namespace std;

// 求最大公约数
int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

class Fraction {
public:
    int numerator;     // 分子
    int denominator;   // 分母

    Fraction(int n = 0, int d = 1) : numerator(n), denominator(d) {}

    // 分数加法
    Fraction operator+(const Fraction &other) const {
        int n = numerator * other.denominator + other.numerator * denominator;
        int d = denominator * other.denominator;
        return Fraction(n, d).reduce();  // 加完后约分
    }

    // 约分函数
    Fraction reduce() const {
        int g = gcd(abs(numerator), abs(denominator));
        return Fraction(numerator / g, denominator / g);
    }
};

int main() {
    int a, b, c, d;
    cin >> a >> b;
    cin >> c >> d;

    Fraction f1(a, b), f2(c, d);

    Fraction sum = f1 + f2;
    cout << sum.numerator << "/" << sum.denominator;

    return 0;
}
