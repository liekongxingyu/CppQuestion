#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

int main() {
    int n;
    std::cin >> n;

    std::vector<long long> a(n);
    for (int i = 0; i < n; ++i) std::cin >> a[i];

    std::vector<long long> sq(n);

    // 对每个区间里的值执行操作，写到第三个参数位置，依次往后
    std::transform(a.begin(), a.end(), sq.begin(),
                   [](long long x) { return x * x; });

    // 把区间值一直和第三个参数累加
    long long sum = std::accumulate(sq.begin(), sq.end(), 0LL);

    std::cout << sum << '\n';
    return 0;
}
