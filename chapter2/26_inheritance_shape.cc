#include <iostream>
#include <iomanip>

class Shape {
public:
    // 不写实现。
    virtual double area() const = 0;   // 纯虚函数
    virtual ~Shape() {}                // 虚析构（保证多态安全释放）
};

class Rectangle : public Shape {
private:
    double width;
    double height;
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    double area() const override {
        return width * height;
    }
};

class Circle : public Shape {
private:
    double radius;
public:
    Circle(double r) : radius(r) {}
    double area() const override {
        return 3.14159 * radius * radius;
    }
};

int main() {
    int type;
    std::cin >> type;

    Shape* p = nullptr;

    if (type == 1) {
        double w, h;
        std::cin >> w >> h;
        // 使用new之后就要delete
        p = new Rectangle(w, h);
    } else if (type == 2) {
        double r;
        std::cin >> r;
        p = new Circle(r);
    }

    std::cout << std::fixed << std::setprecision(2) << p->area() << std::endl;

    delete p;
    return 0;
}
