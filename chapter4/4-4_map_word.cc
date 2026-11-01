#include <iostream>
#include <map>
#include <string>
#include <sstream>

int main() {
    std::string line;
    if (!std::getline(std::cin, line)) return 0;

    std::map<std::string, int> freq;
    // 把line包装成字节流的形式
    std::istringstream iss(line);
    std::string word;

    // >>对字符串的规则是：按空白符分割（空格、多个空格、制表符都行）
    while (iss >> word) {
        ++freq[word];
    }

    for (std::map<std::string, int>::const_iterator it = freq.begin();
         it != freq.end(); ++it) {
        std::cout << it->first << ' ' << it->second << "\n";
    }
    return 0;
}
