#include <iostream>

// N是非类型模板参数，但意义是人为定义的，这里是数组大小的意思
template <typename T, int N>
class MyArray {
private:
    T data[N];

public:
    // 输入 N 个元素
    void input() {
        for (int i = 0; i < N; i++) {
            std::cin >> data[i];
        }
    }

    // 输出 N 个元素
    void print() const {
        for (int i = 0; i < N; i++) {
            std::cout << data[i];
            if (i < N - 1) std::cout << " ";
        }
    }
};

int main() {
    MyArray<int, 5> arr;  // 固定 N = 5

    arr.input();
    arr.print();

    return 0;
}
