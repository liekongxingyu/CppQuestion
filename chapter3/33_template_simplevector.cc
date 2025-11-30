#include <iostream>
using namespace std;

template <typename T>
class SimpleVector {
private:
    T* data;
    int len;

public:
    // 构造函数：开辟空间
    SimpleVector(int n) : len(n) {
        data = new T[n];
    }

    // 下标运算符
    T& operator[](int index) {
        return data[index];
    }

    // size() 函数
    int size() const {
        return len;
    }

    // 析构函数：释放空间
    ~SimpleVector() {
        delete[] data;
    }
};

int main() {
    int n;
    cin >> n;

    SimpleVector<int> vec(n);

    for (int i = 0; i < n; i++) {
        cin >> vec[i];
    }

    for (int i = 0; i < vec.size(); i++) {
        cout << vec[i];
        if (i < vec.size() - 1) cout << " ";
    }

    return 0;
}
