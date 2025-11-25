#include <iostream>

int gcd(int a, int b) {
    while (b != 0) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

int main() {
    int a, b;
    std::cin >> a >> b;

    int g = gcd(a, b);
    int l = a / g * b;

    std::cout << g << " " << l;

    return 0;
}
