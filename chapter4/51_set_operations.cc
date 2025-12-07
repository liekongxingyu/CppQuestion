#include <iostream>
#include <set>
#include <vector>
#include <iterator>
#include <algorithm>

static void print_line(const std::vector<int> &v)
{
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        if (i)
            std::cout << ' ';
        std::cout << v[i];
    }
    std::cout << '\n';
}

int main()
{
    int n;
    std::cin >> n;

    std::set<int> A, B;
    for (int i = 0; i < n; ++i)
    {
        int x;
        std::cin >> x;
        A.insert(x);
    }

    int m;
    std::cin >> m;
    for (int i = 0; i < m; ++i)
    {
        int x;
        std::cin >> x;
        B.insert(x);
    }

    // 用来装结果
    std::vector<int> inters, uni, diff;

    std::set_intersection(A.begin(), A.end(), B.begin(), B.end(),
                          std::back_inserter(inters));
    std::set_union(A.begin(), A.end(), B.begin(), B.end(),
                   std::back_inserter(uni));
    std::set_difference(A.begin(), A.end(), B.begin(), B.end(),
                        std::back_inserter(diff));

    print_line(inters);
    print_line(uni);
    print_line(diff);

    return 0;
}
