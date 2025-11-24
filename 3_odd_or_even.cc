#include <iostream>

int main()
{
    int n = 0;
    // result是一个指向第一个字符串变量的地址,若字符串为"odd",那么*result就是o,但是打印的时候编译器会自动把result解析为那一串字符串的值
    const char *result;

    std::cin >> n;

    if (n % 2 == 0)
    {
        result = "even";
    }
    else
    {
        result = "odd";
    }

    std::cout << result;

    return 0;
}