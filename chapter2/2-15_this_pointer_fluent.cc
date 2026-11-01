#include <iostream>

// 链式调用

class Builder {
private:
    int value;

public:
    Builder() : value(0) {}   // 初始为 0

    Builder& add(int x) {
        value += x;
        return *this;         // 返回自身引用，实现链式调用
    }

    Builder& mul(int x) {
        value *= x;
        return *this;         // 返回自身引用
    }

    void print() const {
        std::cout << value;
    }
};

int main() {
    int a, b, c;
    std::cin >> a >> b >> c;

    Builder builder;

    // ((0 + a) * b + c)
    builder.add(a).mul(b).add(c).print();

    return 0;
}
