#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    cin >> n;

    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    int minVal = arr[0];
    int index = 0;

    for (int i = 1; i < n; i++) {
        if (arr[i] < minVal) {
            minVal = arr[i];
            index = i;
        } else if (arr[i] == minVal) {
            index = i;  // 更新为最后一次出现的位置
        }
    }

    cout << minVal << " " << index << endl;
    return 0;
}
