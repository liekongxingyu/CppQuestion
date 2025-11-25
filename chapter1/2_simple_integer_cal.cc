#include <iostream>

int main()
{
    int a = 0, b = 0;
    char op;

    std::cin >> a >> b;
    std::cin >> op;

    int result = 0;

    switch (op)
    {
    case '+':
        result = a + b;
        break;
    case '-':
        result = a - b;
        break;
    case '*':
        result = a * b;
        break;

    // “/”的逻辑是不四舍五入,如7/2=3
    case '/':
        result = a / b;
        break;
    default:
        return 0;
    }

    std::cout << result;

    return 0;
}