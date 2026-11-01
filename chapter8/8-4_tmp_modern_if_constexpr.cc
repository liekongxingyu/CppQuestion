#include <iostream>
#include <cstdint>
#include <type_traits>

// ============================================================================
// 现代 C++17 的终极进化：if constexpr（编译期分支）
// ============================================================================

// 在旧时代，为了根据类型写不同分支，必须像 81 题那样写好几个 EnableIf 重载结构体。
// C++17 引入 if constexpr，让模板元编程的语法直接回归正常的人类逻辑！

template <typename T>
void smartProcess(T data) {
    // 关键点：if constexpr 是【编译期断舍离】！
    // 编译器在分析 T 的类型后，会把不满足条件的分支彻底从 AST 语法树中直接砍掉！
    if constexpr (std::is_pointer<T>::value) {
        // 如果 T 是 uint32_t，普通 if 编译到 *data 时会直接报语法错误（整型不能解引用）；
        // 但 if constexpr 根本不会去编译这一行代码！
        if (data != nullptr) {
            std::cout << "[if constexpr] Pointer dereferenced: " << *data << '\n';
        } else {
            std::cout << "[if constexpr] Pointer is null.\n";
        }
    } else if constexpr (std::is_integral<T>::value) {
        std::cout << "[if constexpr] Integer double value: " << (data * 2) << '\n';
    } else {
        std::cout << "[if constexpr] Generic fallback handler: " << data << '\n';
    }
}

// 编译期递归累加：用 if constexpr 彻底取代以前的结构体特化
template <uint32_t N>
constexpr uint32_t modernSum() {
    if constexpr (N == 0) {
        return 0; // 递归基（不再需要手写偏特化结构体）
    } else {
        return N + modernSum<N - 1>(); // 编译期递归
    }
}

int main() {
    std::cout << "========== 1. if constexpr 处理不同类型 ==========\n";
    uint32_t number = 50;
    uint32_t* ptr = &number;

    // 分别传入普通整数与指针：同一个函数，编译器按类型分别定制编译
    smartProcess(number); // 只编译整型分支
    smartProcess(ptr);    // 只编译指针分支

    std::cout << "\n========== 2. 用 if constexpr 完成编译期纯算 ==========\n";
    constexpr uint32_t total = modernSum<10>();
    std::cout << "modernSum<10>() = " << total << '\n';
    static_assert(modernSum<10>() == 55, "Compile-time calculation failed!");

    return 0;
}

