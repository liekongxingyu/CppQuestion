#include <iostream>
#include <cstddef>
#include <cstdint>

// 1. nullptr 类型安全重载测试：避免 NULL (0) 匹配整型重载还是指针重载的歧义
void checkPointer(uint32_t value) {
    std::cout << "[checkPointer] Called integer overload with uint32_t: " << value << '\n';
}

void checkPointer(void* ptr) {
    std::cout << "[checkPointer] Called pointer overload with address: " << ptr << '\n';
}

// 2. 基类与面向对象关键字：=default, override, final
class BaseProcessor {
public:
    // = default: 显式指示编译器生成默认构造与虚析构函数
    BaseProcessor() = default;
    virtual ~BaseProcessor() = default;

    // 虚函数
    virtual void processData(uint32_t id) {
        std::cout << "[BaseProcessor] Processing data id: " << id << '\n';
    }

    // 将此函数标记为 final，派生类无法再次重写它
    virtual void lockOperation() final {
        std::cout << "[BaseProcessor] Critical locked operation executed.\n";
    }
};

class ConcreteProcessor : public BaseProcessor {
public:
    // override: 编译器会强制检查基类中是否存在该虚函数，避免因参数写错导致变成新函数
    void processData(uint32_t id) override {
        std::cout << "[ConcreteProcessor] Custom handling for id: " << id << '\n';
    }

    // 如果取消以下注释，编译器会报错：error: virtual function overrides a 'final' function
    // void lockOperation() override {}
};

// final 修饰类：表示该类不能再被任何类继承
class FinalService final : public ConcreteProcessor {
public:
    void processData(uint32_t id) override {
        std::cout << "[FinalService] Terminal processing for id: " << id << '\n';
    }
};

// 如果取消以下注释，编译器会报错：error: cannot derive from 'final' base
// class SubService : public FinalService {};

// 3. = delete：禁止特定操作（如禁止拷贝构造、禁止隐式浮点转换）
class NonCopyableResource {
private:
    uint32_t resource_id_;

public:
    explicit NonCopyableResource(uint32_t id) : resource_id_(id) {}

    // = delete: 显式禁用拷贝构造和拷贝赋值（典型不可拷贝资源类设计）
    NonCopyableResource(const NonCopyableResource&) = delete;
    NonCopyableResource& operator=(const NonCopyableResource&) = delete;

    // 允许移动操作
    NonCopyableResource(NonCopyableResource&&) noexcept = default;
    NonCopyableResource& operator=(NonCopyableResource&&) noexcept = default;

    // = delete 还可以用于禁止不希望发生的隐式类型转换重载
    void setValue(uint32_t val) {
        std::cout << "[NonCopyableResource " << resource_id_ << "] Value set to uint32_t: " << val << '\n';
    }

    // 禁止传入 double/float，防止隐式截断
    void setValue(double) = delete;
};

int main() {
    std::cout << "========== 1. nullptr 类型安全重载解析 ==========\n";
    uint32_t num = 0;
    checkPointer(num);      // 调用 uint32_t 版本
    checkPointer(nullptr);  // 安全无歧义地调用指针版本（如果传入 NULL，在很多编译器会因 0 产生歧义）

    std::cout << "\n========== 2. override 与 final 行为验证 ==========\n";
    uint32_t task_id_1 = 10;
    uint32_t task_id_2 = 20;

    BaseProcessor* p1 = new ConcreteProcessor();
    p1->processData(task_id_1);
    p1->lockOperation();

    BaseProcessor* p2 = new FinalService();
    p2->processData(task_id_2);

    delete p1;
    delete p2;

    std::cout << "\n========== 3. = delete 禁用拷贝与禁用精度截断 ==========\n";
    uint32_t res_id = 999;
    NonCopyableResource res(res_id);

    uint32_t valid_val = 50;
    res.setValue(valid_val);

    // 下面两行如果解开注释，编译时会直接产生 static 错误：
    // NonCopyableResource copy_res = res;   // error: use of deleted function
    // res.setValue(3.14);                   // error: use of deleted function

    std::cout << "NonCopyableResource safely compiled with deleted copy operations.\n";

    return 0;
}
