#include <iostream>
#include <vector>

template <typename T>
class MyQueue
{

private:
    std::vector<T> data;
    int front;

public:
    MyQueue() : front(0) {}

    void push(const T &x)
    {
        data.push_back(x);
    }

    void pop()
    {
        front++;
    }

    T &front()
    {
        return data[front];
    }

    bool empty() const
    {
        return front >= static_cast<int>(data.size());
    }
};

int main()
{
    int n;
    std::cin >> n;

    MyQueue<int> mq;

    for (int i = 0; i < n; i++)
    {
        int x;
        std::cin >> x;
        mq.push(x);
    }
    return 0;
}
