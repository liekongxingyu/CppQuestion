#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

int main()
{
    int n;
    std::cin >> n;

    std::vector<std::pair<int, std::string>> a;
    a.reserve(n);

    for (int i = 0; i < n; ++i)
    {
        int id;
        std::string name;
        std::cin >> id >> name;
        a.push_back(std::make_pair(id, name));
    }

    std::sort(a.begin(), a.end()); // pair 默认先比 first，再比 second

    for (std::size_t i = 0; i < a.size(); ++i)
    {
        std::cout << a[i].first << ' ' << a[i].second << '\n';
    }

    return 0;
}
