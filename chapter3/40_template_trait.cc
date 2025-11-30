#include <iostream>

// 通用模板：默认不是指针
template <typename T>
// 不是结构体，是类型特征的写法，模板元编程的思想
struct is_pointer_type {
    static const bool value = false;
};

// 针对指针类型的偏特化
template <typename T>
struct is_pointer_type<T*> {
    static const bool value = true;
};

int main() {
    std::cout << is_pointer_type<int>::value << std::endl;   // 0
    std::cout << is_pointer_type<int*>::value << std::endl;  // 1
    return 0;
}
