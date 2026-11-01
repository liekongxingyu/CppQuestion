#include <iostream>
#include <cstdint>

// ============================================================================
// 案例 1：用模板实现循环累加 Sum(N) = 1 + 2 + ... + N
// ============================================================================

// 1. 通用模板：相当于普通函数的“递归调用”
// 输入参数是模板参数 N，输出结果是内部的 value 常量
template <uint32_t N>
struct SumToN {
    // 递归：N + Sum(N - 1)
    static constexpr uint32_t value = N + SumToN<N - 1>::value;
};

// 2. 模板特化：相当于递归的“终止条件”（if N == 0 return 0;）
template <>
struct SumToN<0> {
    static constexpr uint32_t value = 0;
};

// ============================================================================
// 案例 2：用模板实现分支与辗转相除法求最大公约数 GCD(A, B)
// ============================================================================

// 通用模板：GCD(A, B) = GCD(B, A % B)
template <uint32_t A, uint32_t B>
struct GCD {
    static constexpr uint32_t value = GCD<B, A % B>::value;
};

// 终止特化：当余数 B 为 0 时，最大公约数即为 A
template <uint32_t A>
struct GCD<A, 0> {
    static constexpr uint32_t value = A;
};

int main() {
    std::cout << "========== 1. 编译期累加 SumToN ==========\n";
    // 编译器在编译这行代码时，自动实例化了 SumToN<10> -> SumToN<9> -> ... -> SumToN<0>
    // 在生成二进制可执行文件前，结果 55 就已经算出来了！
    constexpr uint32_t sum_10 = SumToN<10>::value;
    std::cout << "SumToN<10>::value = " << sum_10 << '\n';

    // 强行证明它在编译期完成：static_assert 只能在编译期运行
    static_assert(SumToN<10>::value == 55, "SumToN calculation mismatch!");

    std::cout << "\n========== 2. 编译期辗转相除法 GCD ==========\n";
    constexpr uint32_t gcd_result = GCD<48, 18>::value;
    std::cout << "GCD<48, 18>::value = " << gcd_result << '\n';
    static_assert(GCD<48, 18>::value == 6, "GCD calculation mismatch!");

    std::cout << "\n========== 3. 把编译期计算结果用作原生数组大小 ==========\n";
    // 原生数组的容量必须在编译期确定，而 GCD<48, 18>::value 就是编译期算出的常量
    uint32_t static_buffer[GCD<48, 18>::value] = {10, 20, 30, 40, 50, 60};
    for (uint32_t i = 0; i < GCD<48, 18>::value; ++i) {
        std::cout << "buffer[" << i << "] = " << static_buffer[i] << '\n';
    }

    return 0;
}

