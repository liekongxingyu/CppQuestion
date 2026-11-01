#include <iostream>
#include <stack>
#include <string>

static bool is_match(char left, char right) {
    return (left == '(' && right == ')') ||
           (left == '[' && right == ']') ||
           (left == '{' && right == '}');
}

int main() {
    std::string s;
    std::cin >> s;

    std::stack<char> st;

    for (std::size_t i = 0; i < s.size(); ++i) {
        char c = s[i];

        if (c == '(' || c == '[' || c == '{') {
            st.push(c);
        } else { // 右括号
            if (st.empty()) {
                std::cout << "NO\n";
                return 0;
            }
            char top = st.top();
            if (!is_match(top, c)) {
                std::cout << "NO\n";
                return 0;
            }
            st.pop();
        }
    }

    std::cout << (st.empty() ? "YES\n" : "NO\n");
    return 0;
}
