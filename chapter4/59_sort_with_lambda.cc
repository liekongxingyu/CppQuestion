#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main()
{
    int n;
    std::cin >> n;

    std::vector<std::string> w(n);
    for (int i = 0; i < n; ++i)
        std::cin >> w[i];

    std::sort(w.begin(), w.end(),
              [](const std::string &a, const std::string &b)
              {
                  if (a.size() != b.size())
                      return a.size() < b.size();
                  return a < b;
              });

    for (int i = 0; i < n; ++i)
    {
        if (i)
            std::cout << ' ';
        std::cout << w[i];
    }
    std::cout << '\n';

    return 0;
}
