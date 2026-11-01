#include <iostream>

int main()
{
    int n = 0;
    int sum = 0;
    std::cin >> n;
    for (int i = 1; i <= n; i++)
    {
        sum += i;
    }

    std::cout << sum << std::endl;

    return 0;
}