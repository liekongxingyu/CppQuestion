#include <iostream>

// 通用模板
template <typename T>
void printValue(T x)
{
    std::cout << x;
}

// 针对 const char* 的特化版本
template <>
void printValue<const char *>(const char *x)
{
    std::cout << "str: " << x;
}

int main()
{
    const char s[101] = "shut the fuck up";

    printValue(s); // 调用特化版本

    return 0;
}
