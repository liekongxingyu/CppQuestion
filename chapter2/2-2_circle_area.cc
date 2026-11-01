#include <iostream>
#include <iomanip>

class Circle {
public:
    double r;

    double area() const {
        return 3.14159 * r * r;
    }
};

int main() {
    Circle c;
    std::cin >> c.r;

    std::cout << std::fixed << std::setprecision(2)
              << c.area();

    return 0;
}
