#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

static std::atomic<long long> counter(0);

static void inc_k_times(int k)
{
    for (int i = 0; i < k; ++i)
    {
        ++counter; // 原子自增（无锁）
    }
}

int main()
{
    int m, k;
    std::cin >> m >> k;

    std::vector<std::thread> threads;
    threads.reserve(m);

    for (int i = 0; i < m; ++i)
    {
        threads.emplace_back(inc_k_times, k);
    }

    for (int i = 0; i < m; ++i)
    {
        threads[i].join();
    }

    std::cout << counter.load() << "\n";
    return 0;
}
