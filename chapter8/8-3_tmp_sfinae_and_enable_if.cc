#include <iostream>
#include <cstdint>
#include <type_traits>

// ============================================================================
// 手写简化版 EnableIf（SFINAE 的核心工具）
// ============================================================================

// 条件为 false 时的默认定义：结构体内部空空如也，没有任何叫 'type' 的东西！
template <bool Condition, typename T = void>
struct MyEnableIf {};

// 条件为 true 时的偏特化版本：内部定义了 using type = T;
template <typename T>
struct MyEnableIf<true, T> {
    using type = T;
};

// ============================================================================
// 案例：利用 SFINAE 让同一个函数名针对不同类型自动“现身”或“隐身”
// ============================================================================

// 重载版本 A：仅当 T 是整数类型时有效（如果不是整数，MyEnableIf 没有 ::type，编译器默默丢弃该版本）
template <typename T>
typename MyEnableIf<std::is_integral<T>::value, void>::type
printSpecialized(T val) {
    std::cout << "[Integral Overload] Direct arithmetic value: " << val << '\n';
}

// 重载版本 B：仅当 T 是指针类型时有效
template <typename T>
typename MyEnableIf<std::is_pointer<T>::value, void>::type
printSpecialized(T ptr) {
    if (ptr != nullptr) {
        std::cout << "[Pointer Overload] Dereferenced value: " << *ptr << '\n';
    } else {
        std::cout << "[Pointer Overload] Null pointer.\n";
    }
}

// 重载版本 C：仅当 T 是浮点数时有效
template <typename T>
typename MyEnableIf<std::is_floating_point<T>::value, void>::type
printSpecialized(T val) {
    std::cout << "[Floating-point Overload] Float formatted: " << val << '\n';
}

int main() {
    std::cout << "========== SFINAE (替换失败不是错误) 与条件重载测试 ==========\n";
    
    // 1. 传入 uint32_t（整数）
    // 编译器尝试版本 B 和 C 时，因为没有 ::type 发生替换失败，但【不报错】，而是安静剔除；
    // 最终唯一命中了版本 A！
    uint32_t int_val = 12345;
    printSpecialized(int_val);

    // 2. 传入指针 uint32_t*
    // 自动命中版本 B
    uint32_t target = 888;
    uint32_t* ptr_val = &target;
    printSpecialized(ptr_val);

    // 3. 传入浮点数 double
    // 自动命中版本 C
    double float_val = 3.14159;
    printSpecialized(float_val);

    return 0;
}

