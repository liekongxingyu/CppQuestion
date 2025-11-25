#include <iostream>

int main() {
    int n;
    std::cin >> n;

    int sum = 0;
    for (int i = 0; i < n; i++) {
        int score;
        std::cin >> score;
        sum += score;
    }

    int avg = sum / n;   // 向下取整

    std::cout << avg << std::endl;

    char grade;
    if (avg >= 90) grade = 'A';
    else if (avg >= 80) grade = 'B';
    else if (avg >= 70) grade = 'C';
    else if (avg >= 60) grade = 'D';
    else grade = 'E';

    std::cout << grade;

    return 0;
}
