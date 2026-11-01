#include <iostream>
using namespace std;

// 模板函数：交换两个变量
template <typename T>
void mySwap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

int main() {
    int a, b;
    cin >> a >> b;
    
    mySwap(a, b);  // 调用模板函数交换
    
    cout << a << " " << b << endl;
    
    return 0;
}
