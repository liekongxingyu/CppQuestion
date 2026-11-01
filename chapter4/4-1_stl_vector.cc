#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int n;
    if (!(std::cin >> n)) return 0;

    std::vector<int> v;

    // 预留n个位置，但不创建元素
    v.reserve(n);

    for (int i = 0; i < n; ++i) {
        int x;
        std::cin >> x;
        if (x % 2 != 0) v.push_back(x); // 只保留奇数
    }

    // 默认是升序，参数一为迭代器起点，参数二为迭代器终点
    std::sort(v.begin(), v.end());

    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i) std::cout << ' ';
        std::cout << v[i];
    }
    // 若没有奇数，则什么也不输出（空行）
    return 0;
}
