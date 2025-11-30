#include <iostream>

template <typename T>
class Stack
{
private:
    T *data;
    int max;
    int top;

public:
    Stack(int m = 1000) : max(m), top(-1)
    {
        data = new T[max];
    }

    void push(const T &value)
    {
        if (top + 1 < max)
        {
            data[++top] = value;
        }
    }
    void pop()
    {
        if (top >= -1)
        {
            --top;
        }
    }
    // 返回值必须是常量或常量引用
    T &top() 
    {
        return data[top];
    }
    ~Stack()
    {
        delete[] data;
    }
};

int main()
{
    int n;
    std::cin >> n;

    Stack<int> st(n); // 栈容量设为 n 即可

    for (int i = 0; i < n; i++)
    {
        int x;
        std::cin >> x;
        st.push(x);
    }

    return 0;
}
