#include <iostream>
#include <vector>

int main() {
    const int h = 2;
    const int w = 3;

    // 创建二维数组的方式
    std::vector<std::vector<int>> m(h, std::vector<int>(w));
    int sum = 0;

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            std::cin >> m[i][j];
            sum += m[i][j];
        }
    }

    std::cout << sum;

    return 0;
}


// #include <iostream>

// int main()
// {
//     const int h = 2;
//     const int w = 3;
//     int sum = 0;

//     int m[h][w];

//     for (int i = 0; i < h; i++)
//     {
//         for (int j = 0; j < w; j++)
//         {
//             std::cin >> m[i][j];
//             sum += m[i][j];
//         }
//     }
//     std::cout << sum;

//     return 0;
// }