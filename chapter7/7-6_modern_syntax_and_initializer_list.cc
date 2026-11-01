#include <iostream>
#include <vector>
#include <initializer_list>
#include <type_traits>
#include <cstdint>

// 自定义类支持统一初始化列表（std::initializer_list）
class NumberCollection {
private:
    std::vector<uint32_t> numbers_;

public:
    // 接受 std::initializer_list<uint32_t> 的构造函数，支持 {1, 2, 3} 初始化语法
    NumberCollection(std::initializer_list<uint32_t> init_list) {
        for (uint32_t val : init_list) { // 范围 for 循环
            numbers_.push_back(val);
        }
    }

    // 暴露只读迭代器，使其支持外部范围 for 循环
    auto begin() const { return numbers_.begin(); }
    auto end() const { return numbers_.end(); }

    uint32_t size() const { return static_cast<uint32_t>(numbers_.size()); }
};

int main() {
    std::cout << "========== 1. std::initializer_list 列表初始化 ==========\n";
    // 统一初始化语法
    NumberCollection collection = {10u, 20u, 30u, 40u, 50u};
    std::cout << "Collection size: " << collection.size() << '\n';

    std::cout << "\n========== 2. 基于范围的 for 循环 (Range-based for loop) ==========\n";
    std::cout << "Items in collection: ";
    for (uint32_t val : collection) {
        std::cout << val << ' ';
    }
    std::cout << '\n';

    std::cout << "\n========== 3. auto 自动类型推导 ==========\n";
    uint32_t base_num = 100;
    auto auto_val = base_num;            // 推导为 uint32_t
    const auto& auto_ref = base_num;     // 推导为 const uint32_t&
    auto* auto_ptr = &base_num;          // 推导为 uint32_t*

    static_assert(std::is_same<decltype(auto_val), uint32_t>::value, "auto_val must be uint32_t");
    static_assert(std::is_same<decltype(auto_ref), const uint32_t&>::value, "auto_ref must be const uint32_t&");
    static_assert(std::is_same<decltype(auto_ptr), uint32_t*>::value, "auto_ptr must be uint32_t*");

    std::cout << "auto_val: " << auto_val << ", *auto_ptr: " << *auto_ptr << '\n';

    std::cout << "\n========== 4. decltype 类型萃取与推导 ==========\n";
    uint32_t x = 15;
    uint32_t y = 25;
    decltype(x + y) sum_res = x + y; // decltype 推导表达式 (x + y) 的类型为 uint32_t
    static_assert(std::is_same<decltype(sum_res), uint32_t>::value, "sum_res must be uint32_t");

    std::cout << "decltype(x + y) sum_res = " << sum_res << '\n';

    std::cout << "\n========== 5. 原生数组与 vector 的范围 for 修改 ==========\n";
    uint32_t raw_array[] = {1, 2, 3, 4};
    // 使用引用可以就地修改元素
    for (auto& item : raw_array) {
        item *= 2;
    }
    std::cout << "Doubled raw_array: ";
    for (uint32_t item : raw_array) {
        std::cout << item << ' ';
    }
    std::cout << '\n';

    return 0;
}
