#include <iostream>
#include <string>
using namespace std;

int main()
{
    string str;
    getline(cin, str);

    int n = str.size();
    string str2;
    // 需要先分配空间
    str2.resize(n);   // 分配空间

    int i = 0;
    int j = n - 1;

    while (i < n)
    {
        str2[j] = str[i];
        i++;
        j--;
    }

    cout << str2;
    return 0;
}

// 标准做法
// #include <iostream>
// #include <string>
// #include <algorithm>
// using namespace std;

// int main()
// {
//     string str;
//     getline(cin, str);

//     string str2 = str;
//     reverse(str2.begin(), str2.end());

//     cout << str2;
// }
