#include <iostream>
#include <vector>
#include <thread>

static void partial_sum(const std::vector<long long>& a, int l, int r, long long* out) {
    long long s = 0;
    for (int i = l; i < r; ++i) s += a[i];
    *out = s;
}

int main() {
    int n;
    std::cin >> n;

    std::vector<long long> a(n);
    for (int i = 0; i < n; ++i) std::cin >> a[i];

    int mid = n / 2;
    long long s1 = 0, s2 = 0;

    // cref会把a包装成引用传进去
    std::thread t1(partial_sum, std::cref(a), 0, mid, &s1);
    std::thread t2(partial_sum, std::cref(a), mid, n, &s2);

    // 让主线程等待这两个线程执行完毕再执行
    t1.join();
    t2.join();

    std::cout << (s1 + s2) << "\n";
    return 0;
}
