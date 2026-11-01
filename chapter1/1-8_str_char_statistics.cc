#include <iostream>
#include <string>

int main()
{
    std::string s;
    // cin会丢弃空白字符
    getline(std::cin, s); // 读取一整行，包括空格

    int letters = 0;
    int digits = 0;
    int spaces = 0;

    std::cout << (int)s[1] << std::endl;

    for (char c : s)
    {
        if (isalpha(c))
        {
            letters++;
        }
        else if (isdigit(c))
        {
            digits++;
        }
        else if (c == ' ')
        {
            spaces++;
        }
    }

    std::cout << letters << " " << digits << " " << spaces;

    return 0;
}
