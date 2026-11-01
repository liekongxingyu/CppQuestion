#include <iostream>
#include <utility>
#include <vector>
#include <cstdint>

class DynamicArray {
private:
    uint32_t size_;
    uint32_t* data_;

public:
    // 构造函数
    explicit DynamicArray(uint32_t size) : size_(size), data_(new uint32_t[size]{}) {
        for (uint32_t i = 0; i < size_; ++i) {
            data_[i] = i + 1;
        }
        std::cout << "[Constructor] Allocated array of size: " << size_ << '\n';
    }

    // 析构函数
    ~DynamicArray() {
        if (data_ != nullptr) {
            delete[] data_;
            std::cout << "[Destructor] Released array of size: " << size_ << '\n';
        } else {
            std::cout << "[Destructor] Empty array destroyed\n";
        }
    }

    // 传统深拷贝构造函数（C++98 风格，深拷贝开销大）
    DynamicArray(const DynamicArray& other) : size_(other.size_), data_(new uint32_t[other.size_]) {
        for (uint32_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
        std::cout << "[Copy Constructor] Deep copy performed for size: " << size_ << '\n';
    }

    // 传统深拷贝赋值运算符
    DynamicArray& operator=(const DynamicArray& other) {
        std::cout << "[Copy Assignment] Deep copy assignment for size: " << other.size_ << '\n';
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new uint32_t[size_];
            for (uint32_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }

    // C++11 移动构造函数（Move Constructor）：窃取右值资源，无内存分配，常数级开销
    // 标明 noexcept 以便 std::vector 在扩容时优先使用移动构造
    DynamicArray(DynamicArray&& other) noexcept : size_(other.size_), data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr;
        std::cout << "[Move Constructor] Resource stolen! Size: " << size_ << '\n';
    }

    // C++11 移动赋值运算符（Move Assignment Operator）
    DynamicArray& operator=(DynamicArray&& other) noexcept {
        std::cout << "[Move Assignment] Resource stolen via assignment!\n";
        if (this != &other) {
            delete[] data_; // 释放自身原有资源
            size_ = other.size_;
            data_ = other.data_; // 接管资源
            other.size_ = 0;
            other.data_ = nullptr;
        }
        return *this;
    }

    uint32_t size() const { return size_; }

    void print() const {
        std::cout << "Array content (size=" << size_ << "): ";
        for (uint32_t i = 0; i < size_; ++i) {
            std::cout << data_[i] << ' ';
        }
        std::cout << '\n';
    }
};

// 产生右值的工厂函数
DynamicArray createArray(uint32_t count) {
    DynamicArray temp(count);
    return temp; // 返回局部对象，触发 RVO 或移动语义
}

int main() {
    std::cout << "========== 1. 传统深拷贝 ==========\n";
    uint32_t initial_size = 3;
    DynamicArray arr1(initial_size);
    DynamicArray arr2 = arr1; // 拷贝构造
    arr1.print();
    arr2.print();

    std::cout << "\n========== 2. 右值引用与 std::move 触发移动构造 ==========\n";
    // std::move 并不会真正“移动”任何东西，而是强制将左值转换为右值引用
    DynamicArray arr3 = std::move(arr1);
    std::cout << "After std::move, arr1.size() = " << arr1.size() << '\n';
    arr3.print();

    std::cout << "\n========== 3. 移动赋值测试 ==========\n";
    uint32_t new_size = 4;
    DynamicArray arr4(new_size);
    arr4 = std::move(arr3); // 触发移动赋值
    arr4.print();

    std::cout << "\n========== 4. 函数返回值与移动语义 ==========\n";
    uint32_t factory_count = 5;
    DynamicArray arr5 = createArray(factory_count);
    arr5.print();

    std::cout << "\n========== 5. 退出作用域统一析构 ==========\n";
    return 0;
}
