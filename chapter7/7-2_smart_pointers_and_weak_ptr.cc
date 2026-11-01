#include <iostream>
#include <memory>
#include <vector>
#include <cstdint>

class Device {
private:
    uint32_t id_;

public:
    explicit Device(uint32_t id) : id_(id) {
        std::cout << "[Device " << id_ << "] Created\n";
    }

    ~Device() {
        std::cout << "[Device " << id_ << "] Destroyed\n";
    }

    void operate() const {
        std::cout << "[Device " << id_ << "] Operating...\n";
    }

    uint32_t id() const { return id_; }
};

// 演示循环引用的节点
struct BadNode {
    uint32_t val;
    std::shared_ptr<BadNode> next;

    explicit BadNode(uint32_t v) : val(v) {
        std::cout << "[BadNode " << val << "] Created\n";
    }
    ~BadNode() {
        std::cout << "[BadNode " << val << "] Destroyed\n";
    }
};

// 使用 std::weak_ptr 避免循环引用的健康节点
struct GoodNode {
    uint32_t val;
    std::weak_ptr<GoodNode> next; // 弱引用，不增加引用计数

    explicit GoodNode(uint32_t v) : val(v) {
        std::cout << "[GoodNode " << val << "] Created\n";
    }
    ~GoodNode() {
        std::cout << "[GoodNode " << val << "] Destroyed\n";
    }
};

int main() {
    std::cout << "========== 1. std::unique_ptr 独占指针与所有权转移 ==========\n";
    {
        uint32_t device_id_1 = 101;
        std::unique_ptr<Device> uptr1 = std::make_unique<Device>(device_id_1);
        uptr1->operate();

        // unique_ptr 不能被拷贝，只能通过 std::move 转移所有权
        std::unique_ptr<Device> uptr2 = std::move(uptr1);
        if (uptr1 == nullptr) {
            std::cout << "uptr1 is now nullptr after move\n";
        }
        uptr2->operate();
        // 离开作用域，uptr2 自动析构 Device 101
    }

    std::cout << "\n========== 2. std::shared_ptr 共享指针与引用计数 ==========\n";
    {
        uint32_t device_id_2 = 202;
        std::shared_ptr<Device> sptr1 = std::make_shared<Device>(device_id_2);
        std::cout << "sptr1 use_count: " << static_cast<uint32_t>(sptr1.use_count()) << '\n';

        {
            std::shared_ptr<Device> sptr2 = sptr1; // 共享所有权
            std::cout << "Inside inner scope, use_count: " << static_cast<uint32_t>(sptr1.use_count()) << '\n';
            sptr2->operate();
        } // sptr2 离开作用域，引用计数减 1

        std::cout << "After inner scope, use_count: " << static_cast<uint32_t>(sptr1.use_count()) << '\n';
    } // sptr1 离开作用域，引用计数归零，Device 202 自动析构

    std::cout << "\n========== 3. std::shared_ptr 循环引用陷阱（导致内存泄漏） ==========\n";
    {
        uint32_t node_val_1 = 1;
        uint32_t node_val_2 = 2;
        auto a = std::make_shared<BadNode>(node_val_1);
        auto b = std::make_shared<BadNode>(node_val_2);

        a->next = b; // a 持有 b
        b->next = a; // b 持有 a，形成循环引用

        std::cout << "Cyclic ref: a use_count=" << static_cast<uint32_t>(a.use_count())
                  << ", b use_count=" << static_cast<uint32_t>(b.use_count()) << '\n';
        std::cout << "Scope ending: notice that BadNodes will NOT be destroyed!\n";
    } // 此时离开作用域，a 与 b 互相持有对方的 shared_ptr，use_count 均变为 1，析构函数永远无法触发！

    std::cout << "\n========== 4. std::weak_ptr 破除循环引用 ==========\n";
    {
        uint32_t good_val_1 = 10;
        uint32_t good_val_2 = 20;
        auto ga = std::make_shared<GoodNode>(good_val_1);
        auto gb = std::make_shared<GoodNode>(good_val_2);

        ga->next = gb; // weak_ptr 不增加引用计数
        gb->next = ga; // weak_ptr 不增加引用计数

        std::cout << "Weak ref: ga use_count=" << static_cast<uint32_t>(ga.use_count())
                  << ", gb use_count=" << static_cast<uint32_t>(gb.use_count()) << '\n';

        // 访问 weak_ptr 所指对象时需通过 lock() 提升为 shared_ptr
        if (auto shared_b = ga->next.lock()) {
            std::cout << "Successfully accessed GoodNode via lock(), val=" << shared_b->val << '\n';
        }
        std::cout << "Scope ending: GoodNodes will be properly destroyed:\n";
    } // 离开作用域，引用计数正常归零，所有 GoodNode 均能正常析构

    return 0;
}
