#include <iostream>
#include <unordered_map>
#include <string>

int main() {
    int n;
    if (!(std::cin >> n)) return 0;

    // 哈希表结构的键值对容器
    std::unordered_map<std::string, std::string> dict;

    // 转换为size_t无符号类型
    dict.reserve(static_cast<std::size_t>(n));

    for (int i = 0; i < n; ++i) {
        std::string en, cn;
        std::cin >> en >> cn;
        dict[en] = cn;
    }

    std::string q;
    std::cin >> q;

    // 找到了就返回一个指向该键值对的迭代器，没找到就返回dict.end()
    std::unordered_map<std::string, std::string>::const_iterator it = dict.find(q);
    if (it != dict.end()) {
        std::cout << it->second;
    } else {
        std::cout << "not found";
    }
    return 0;
}
