#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

static int counter = 0;
static std::mutex counter_mtx;

static void inc_k_times(int k)
{
    for (int i = 0; i < k; i++)
    {
        // 作用域结束会自动释放锁
        std::lock_guard<std::mutex> lock(counter_mtx);
        ++counter;
    }
}

int main()
{
    int m, k;
    std::cin >> m >> k;

    std::vector<std::thread> threads;
    threads.reserve(m);

    for (int i = 0; i < m; i++)
    {
        threads.emplace_back(inc_k_times, k);
    }

    for (int i = 0; i < m; ++i)
    {
        threads[i].join();
    }

    std::cout << counter << "\n";

    return 0;
}