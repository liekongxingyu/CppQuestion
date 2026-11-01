#include <iostream>
#include <vector>

template <typename T>
class Matrix {
private:
    std::vector<std::vector<T>> data;

public:
    // 默认构造：创建 2×2，初始化为 0
    Matrix() : data(2, std::vector<T>(2, T())) {}

    // 用四个值初始化 2×2
    Matrix(T a, T b, T c, T d) {
        data = {{a, b}, {c, d}};
    }

    // add：返回新的矩阵
    Matrix add(const Matrix& other) const {
        Matrix result;
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                result.data[i][j] = data[i][j] + other.data[i][j];
        return result;
    }

    // 打印
    void print() const {
        for (int i = 0; i < 2; i++) {
            std::cout << data[i][0] << " " << data[i][1] << "\n";
        }
    }
};

int main() {
    int a1, a2, a3, a4;
    int b1, b2, b3, b4;

    std::cin >> a1 >> a2 >> a3 >> a4;
    std::cin >> b1 >> b2 >> b3 >> b4;

    Matrix<int> m1(a1, a2, a3, a4);
    Matrix<int> m2(b1, b2, b3, b4);

    Matrix<int> result = m1.add(m2);

    result.print();

    return 0;
}
