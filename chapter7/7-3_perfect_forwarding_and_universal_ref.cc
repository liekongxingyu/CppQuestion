#include <iostream>
#include <utility>
#include <memory>
#include <cstdint>

// 目标重载函数：区分接收的是左值还是右值
void processValue(uint32_t& x) {
    std::cout << "[processValue] Lvalue reference: " << x << '\n';
}

void processValue(const uint32_t& x) {
    std::cout << "[processValue] Const lvalue reference: " << x << '\n';
}

void processValue(uint32_t&& x) {
    std::cout << "[processValue] Rvalue reference: " << x << '\n';
}

// 错误示范：没有完美转发，命名右值引用形参在函数体内会蜕化为左值
template <typename T>
void badForward(T&& arg) {
    std::cout << "-> Inside badForward: ";
    processValue(arg); // 无论外部传左值还是右值，arg 作为有名字的变量，都是左值！
}

// 正确示范：使用 std::forward 实现完美转发（保留原始的左/右值属性）
template <typename T>
void perfectForward(T&& arg) {
    std::cout << "-> Inside perfectForward: ";
    processValue(std::forward<T>(arg)); // 引用折叠 + 条件转换
}

// 实际工程应用：通用的就地构造工厂包装器
class Packet {
private:
    uint32_t id_;
    uint32_t payload_size_;

public:
    Packet(uint32_t id, uint32_t size) : id_(id), payload_size_(size) {
        std::cout << "[Packet Created] ID=" << id_ << ", Size=" << payload_size_ << '\n';
    }

    void show() const {
        std::cout << "[Packet Info] ID: " << id_ << ", Payload: " << payload_size_ << " bytes\n";
    }
};

// 类似 std::make_unique 的通用转发工厂函数
template <typename T, typename... Args>
std::unique_ptr<T> createInstance(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

int main() {
    std::cout << "========== 1. 未使用 std::forward 的情况（右值退化为左值） ==========\n";
    uint32_t num = 42;
    badForward(num);          // 传入左值 -> 调用 lvalue 版本
    badForward(100u);         // 传入右值 -> 依然错误调用 lvalue 版本！

    std::cout << "\n========== 2. 使用 std::forward 的完美转发 ==========\n";
    perfectForward(num);                 // 传入左值 -> 正确调用 lvalue 版本
    perfectForward(100u);                // 传入右值 -> 正确调用 rvalue 版本
    const uint32_t const_num = 99;
    perfectForward(const_num);           // 传入 const 左值 -> 正确调用 const lvalue 版本
    perfectForward(std::move(num));      // 显式 std::move 转为右值 -> 正确调用 rvalue 版本

    std::cout << "\n========== 3. 工程案例：变长参数模板 + 完美转发构造对象 ==========\n";
    uint32_t packet_id = 1;
    uint32_t packet_size = 1024;
    auto packet_ptr = createInstance<Packet>(packet_id, packet_size);
    packet_ptr->show();

    return 0;
}
