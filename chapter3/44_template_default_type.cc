#include <iostream>

template <typename T = int>   // 默认模板类型为 int
class Box {
private:
    T value;

public:
    // 构造函数
    Box(const T& v) : value(v) {}

    // 输出 value
    void print() const {
        std::cout << value << std::endl;
    }
};

int main() {
    int a;
    double b;

    std::cin >> a;
    std::cin >> b;

    // 使用默认类型 int
    Box<> box1(a);
    box1.print();

    // 指定类型 double
    Box<double> box2(b);
    box2.print();

    return 0;
}
