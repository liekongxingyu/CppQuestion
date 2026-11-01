#include <iostream>
#include <cstdint>

// ============================================================================
// 模板元编程的核心思想：把“类型”当作变量来计算
// ============================================================================

// 1. 判断一个类型是不是指针（手写简易版 std::is_pointer）
// 默认通用模板：认为不是指针
template <typename T>
struct IsPointer {
    static constexpr bool value = false;
};

// 偏特化版本：当且仅当传入的是 T* 时，精确匹配这个版本！
template <typename T>
struct IsPointer<T*> {
    static constexpr bool value = true;
};

// 2. 类型转换：剥除指针（手写简易版 std::remove_pointer）
// 输入是一个类型 T，输出是内部的 using type
template <typename T>
struct RemovePointer {
    using type = T; // 如果不是指针，原样返回
};

// 偏特化版本：如果是 T*，只把里面的 T 提取出来
template <typename T>
struct RemovePointer<T*> {
    using type = T; // 剥掉一层星号
};

// 3. 判断两个类型是否相同（手写简易版 std::is_same）
template <typename T, typename U>
struct IsSame {
    static constexpr bool value = false;
};

// 偏特化：只有当两个类型完全相同时匹配
template <typename T>
struct IsSame<T, T> {
    static constexpr bool value = true;
};

// 演示：一个通用打印函数，利用模板元编程在编译期得知类型信息
template <typename T>
void inspectType(T) {
    if (IsPointer<T>::value) {
        std::cout << "[inspectType] Argument is a POINTER type.\n";
    } else {
        std::cout << "[inspectType] Argument is a NON-POINTER type.\n";
    }
}

int main() {
    std::cout << "========== 1. 编译期类型判断 IsPointer ==========\n";
    // 我们可以直接询问编译器某个类型的属性！
    std::cout << "IsPointer<uint32_t>::value  = " << std::boolalpha << IsPointer<uint32_t>::value << '\n';
    std::cout << "IsPointer<uint32_t*>::value = " << std::boolalpha << IsPointer<uint32_t*>::value << '\n';

    uint32_t number = 42;
    uint32_t* ptr = &number;
    inspectType(number);
    inspectType(ptr);

    std::cout << "\n========== 2. 编译期类型剥除 RemovePointer ==========\n";
    // 声明一个类型：把 uint32_t* 的指针剥掉，得到 uint32_t
    using CleanType = typename RemovePointer<uint32_t*>::type;
    CleanType regular_num = 100; // regular_num 就是普通的 uint32_t
    std::cout << "regular_num = " << regular_num << ", sizeof = " << sizeof(regular_num) << " bytes\n";

    std::cout << "\n========== 3. 编译期类型比对 IsSame ==========\n";
    std::cout << "IsSame<CleanType, uint32_t>::value  = " << IsSame<CleanType, uint32_t>::value << '\n';
    std::cout << "IsSame<CleanType, uint32_t*>::value = " << IsSame<CleanType, uint32_t*>::value << '\n';

    // 编译期静态断言验证
    static_assert(IsSame<CleanType, uint32_t>::value, "CleanType must be uint32_t!");
    std::cout << "All type trait static assertions passed!\n";

    return 0;
}

