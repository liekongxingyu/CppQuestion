#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

static std::queue<int> q;
static std::mutex mtx;
static std::condition_variable cv;
static bool done = false;

static void producer(int n)
{
    for (int i = 1; i <= n; i++)
    {
        {
            // lock_guard是锁管理器
            std::lock_guard<std::mutex> lock(mtx);
            q.push(i);
        }

        cv.notify_one();
        {
            std::lock_guard<std::mutex> lock(mtx);
            done = true;
        }
        cv.notify_one();
    }
}

static void consumer()
{
    while (true)
    {
        // 可以手动加/解锁
        std::unique_lock<std::mutex> lock(mtx);
        // 如果队列为空，则自动 unlock(mtx)，然后睡眠等待通知
        cv.wait(lock, []
                { return !q.empty() || done; });

        while (!q.empty())
        {
            int x = q.front();
            q.pop();
            lock.unlock();
            std::cout << x << "\n";
            lock.lock();
        }
        if (done && q.empty())
            break;
    }
}

int main()
{
    int n;
    std::cin >> n;

    std::thread p(producer, n);
    std::thread c(consumer);

    p.join();
    c.join();
}