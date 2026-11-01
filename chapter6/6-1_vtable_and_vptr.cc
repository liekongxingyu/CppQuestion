#include <iostream>
#include <cstdint>
#include <climits>

class Base {
public:
    virtual void f() {
        std::cout << "Base::f()" << std::endl;
    }

    virtual void g() {
        std::cout << "Base::g()" << std::endl;
    }

    virtual ~Base() = default;
};

int main() {
    Base b;

    // 1. 获取并输出对象的首地址
    std::cout << "Object address: " << &b << std::endl;

    // 2. 获取对象起始位置的虚表指针 (vptr) 所指向的虚函数表 (vtable) 首地址
    // 对象的首地址处存储的是 vptr 指针（指向虚函数表数组）
    uintptr_t* vptr = *(uintptr_t**)&b;
    std::cout << "Vtable address: " << vptr << std::endl;

    // 3. 定义函数指针类型：无参且无返回值的函数
    using Func = void (*)();

    // 4. 从虚函数表中依次读取函数指针并调用
    // vptr[0] 对应虚函数表中第 1 个函数 Base::f()
    // vptr[1] 对应虚函数表中第 2 个函数 Base::g()
    Func pFunc1 = reinterpret_cast<Func>(vptr[0]);
    Func pFunc2 = reinterpret_cast<Func>(vptr[1]);

    pFunc1();
    pFunc2();

    return 0;
}
