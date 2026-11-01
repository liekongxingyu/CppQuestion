#include <iostream>

// 模板元编程求阶乘
template <int N>
struct Factorial {
    static const int value = N * Factorial<N - 1>::value;
};

// 特化：0! = 1
template <>
struct Factorial<0> {
    // 写成static可以直接通过类名访问，const是因为编译期只能是常量
    static const int value = 1;
};

int main() {
    std::cout << Factorial<5>::value << std::endl;
    return 0;
}
