#include <iostream>
#include <set>

int main()
{
    int n;
    std::cin >> n;

    // 集合是有序的，multiset允许重复元素
    std::multiset<int> ms;
    for (int i = 0; i < n; ++i)
    {
        int x;
        std::cin >> x;
        ms.insert(x);
    }

    for (std::multiset<int>::iterator it = ms.begin(); it != ms.end();)
    {
        // *it是取出它指代的元素值
        int val = *it;
        // count(x)返回x在multiset中出现了多少次
        int cnt = static_cast<int>(ms.count(val));
        std::cout << val << ' ' << cnt << '\n';
        // upper_bound返回一个迭代器，指向第一个大于val的值
        it = ms.upper_bound(val); // 跳到下一个不同的值
    }

    return 0;
}
