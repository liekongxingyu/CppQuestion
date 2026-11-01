#include <iostream>
#include <string>

int main() {
    std::string s;
    std::cin >> s;

    bool ok = true;
    std::size_t i = 0;
    // s.size()返回的就是size_t
    std::size_t j = s.size();
    if (j > 0) j -= 1;

    while (i < j) {
        if (s[i] != s[j]) {
            ok = false;
            break;
        }
        ++i;
        --j;
    }

    std::cout << (ok ? "YES" : "NO") << '\n';
    return 0;
}
