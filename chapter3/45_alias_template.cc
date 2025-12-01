#include <iostream>
#include <map>
#include <string>

// 模板别名：IntMap<T> 等价于 std::map<int, T>
template <typename T>
// map的原型为std::mao<key,value>,这里的意思是固定key为int类型的
using IntMap = std::map<int, T>;

int main() {
    int n;
    std::cin >> n;

    IntMap<std::string> mp;  // 使用模板别名

    for (int i = 0; i < n; i++) {
        int key;
        std::string value;
        std::cin >> key >> value;
        mp[key] = value;  // 插入键值对
    }

    // map 自动按 key 升序
    for (const auto& p : mp) {
        std::cout << p.first << " " << p.second << std::endl;
    }

    return 0;
}
