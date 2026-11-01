#include <iostream>
#include <thread>

static void print_numbers(int n) {
    for (int i = 1; i <= n; ++i) {
        std::cout << i << '\n';
    }
}

int main() {
    int n;
    std::cin >> n;

    std::thread t(print_numbers, n);
    t.join(); // 主线程等待子线程结束

    return 0;
}
