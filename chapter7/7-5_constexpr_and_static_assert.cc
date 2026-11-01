#include <iostream>
#include <cstdint>

// C++11 constexpr 函数：当所有传入实参为编译期常量时，在编译期完成求值
constexpr uint32_t compileTimeFactorial(uint32_t n) {
    return (n <= 1) ? 1 : n * compileTimeFactorial(n - 1);
}

constexpr uint32_t compileTimePower(uint32_t base, uint32_t exp) {
    return (exp == 0) ? 1 : base * compileTimePower(base, exp - 1);
}

// 固定大小缓冲结构体，演示使用 constexpr 计算出的值作为静态数组长度
template <uint32_t N>
struct StaticStorage {
    uint32_t buffer[N];
    constexpr uint32_t capacity() const { return N; }
};

int main() {
    std::cout << "========== 1. constexpr 编译期常量计算 ==========\n";
    // 编译期常量
    constexpr uint32_t base_val = 2;
    constexpr uint32_t power_exp = 8;
    constexpr uint32_t power_res = compileTimePower(base_val, power_exp); // 编译期计算出 256
    constexpr uint32_t fact_5 = compileTimeFactorial(5);                 // 编译期计算出 120

    std::cout << base_val << "^" << power_exp << " = " << power_res << '\n';
    std::cout << "5! = " << fact_5 << '\n';

    std::cout << "\n========== 2. static_assert 编译期断言 ==========\n";
    // 编译期静态断言，如果不满足条件，在编译阶段直接报错中断，不会产生任何运行期开销
    static_assert(compileTimePower(2, 8) == 256, "Error: 2^8 must be 256");
    static_assert(compileTimeFactorial(5) == 120, "Error: 5! must be 120");
    static_assert(sizeof(uint32_t) == 4, "Warning: uint32_t must be exactly 4 bytes (32-bit)");
    std::cout << "All static_assert assertions passed during compilation!\n";

    std::cout << "\n========== 3. 使用 constexpr 结果定义模板实参及数组大小 ==========\n";
    // fact_5 是编译期常量，可以直接用于模板非类型参数以及原生数组大小
    constexpr uint32_t buffer_size = compileTimeFactorial(3); // 3! = 6
    StaticStorage<buffer_size> storage{};
    for (uint32_t i = 0; i < storage.capacity(); ++i) {
        storage.buffer[i] = (i + 1) * 10;
    }

    std::cout << "Storage buffer (capacity=" << storage.capacity() << "): ";
    for (uint32_t i = 0; i < storage.capacity(); ++i) {
        std::cout << storage.buffer[i] << ' ';
    }
    std::cout << '\n';

    std::cout << "\n========== 4. constexpr 函数同时支持运行期普通调用 ==========\n";
    uint32_t runtime_input = 4;
    // 传入运行期变量，自动降级为普通运行期函数调用
    uint32_t runtime_result = compileTimeFactorial(runtime_input);
    std::cout << "Runtime calculated " << runtime_input << "! = " << runtime_result << '\n';

    return 0;
}
