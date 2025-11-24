#include <iostream>

int main()
{
    int n = 0;
    int a = 0, b = 1, c;
    std::cin >> n;
    if (n == 0)
    {
        c = 0;
        std::cout << c;
        return 0;
    }
    if (n == 1)
    {
        c = 1;
        std::cout << c;
        return 0;
    }

    for (int i = 2; i <= n; i++)
    {
        c = a + b;
        a = b;
        b = c;
    }

    std::cout << b;

    return 0;
}