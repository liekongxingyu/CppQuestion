#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    int n;
    if (!(std::cin >> n))
        return 0;

    std::vector<int> v;
    v.reserve(n);
    for (int i = 0; i < n; ++i)
    {
        int x;
        std::cin >> x;
        v.push_back(x);
    }

    std::sort(v.begin(), v.end());

    // unique会返回一个迭代器，函数效果：(1,2,1,1,3)->(1,2,1,3)
    auto d = std::unique(v.begin(), v.end());

    // 左闭右开
    v.erase(d, v.end());

    for (std::size_t i = 0; i < v.size(); ++i)
    {
        if (i)
            std::cout << ' ';
        std::cout << v[i];
    }
    return 0;
}
