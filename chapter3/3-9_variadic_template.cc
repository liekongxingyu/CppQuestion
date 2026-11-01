#include <iostream>

// 终止函数：当只剩一个参数时直接返回
template <typename T>
T sum(T x) {
    return x;
}

// 可变参数模板：一个参数 + 剩余参数
template <typename T, typename... Args>
T sum(T x, Args... args) {
    return x + sum(args...);
}

int main() {
    int a, b, c;
    std::cin >> a >> b >> c;

    std::cout << sum(a, b, c);

    return 0;
}
