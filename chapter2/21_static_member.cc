#include <iostream>

class Counter
{
public:

    // 多个实例对象共享同一变量
    static int count;

    Counter()
    {
        count++;
    }
};

// 静态成员变量必须在类外定义
int Counter::count = 0;

int main()
{
    int n;
    std::cin >> n;

    for (int i = 0; i < n; i++)
    {
        Counter c;
    }

    std::cout << Counter::count;

    return 0;
}
