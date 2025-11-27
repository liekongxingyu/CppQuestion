#include <iostream>
#include <string>

template <typename T>
T myMax(const T& a, const T& b) {
    return (a > b) ? a : b;
}

int main() {
    int x, y;
    std::cin >> x >> y;

    std::cout << myMax(x, y);

    return 0;
}
