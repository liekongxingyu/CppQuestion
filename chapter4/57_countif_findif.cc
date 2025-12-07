#include <iostream>
#include <vector>
#include <algorithm>

struct GreaterThanZero {
    bool operator()(int x) const { return x > 0; }
};

struct LessThanZero {
    bool operator()(int x) const { return x < 0; }
};

int main() {
    int n;
    std::cin >> n;

    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) std::cin >> a[i];

    // countif是返回所有区间内第三个参数为true的数量，findif是返回第一个（没找到就返回最后一个）
    int cnt = static_cast<int>(std::count_if(a.begin(), a.end(), GreaterThanZero()));
    std::cout << cnt << '\n';

    std::vector<int>::iterator it = std::find_if(a.begin(), a.end(), LessThanZero());
    if (it == a.end()) {
        std::cout << -1 << '\n';
    } else {
        int idx = static_cast<int>(it - a.begin()); // vector 支持随机访问，迭代器可相减
        std::cout << idx << '\n';
    }

    return 0;
}
