#include <iostream>

int main() {
    int n;
    std::cin >> n;

    if (n <= 1) {
        std::cout << "NO";
        return 0;
    }

    bool prime = true;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            prime = false;
            break;
        }
    }

    std::cout << (prime ? "YES" : "NO");

    return 0;
}
