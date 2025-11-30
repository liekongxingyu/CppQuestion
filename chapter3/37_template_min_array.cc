#include <iostream>

// 函数模板：返回数组中最小值
template <typename T>
T getMin(T arr[], int n) {
    T minVal = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] < minVal) {
            minVal = arr[i];
        }
    }
    return minVal;
}

int main() {
    int n;
    std::cin >> n;

    int arr[1010];

    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }

    int result = getMin(arr, n);

    std::cout << result;

    return 0;
}
