#include <iostream>
#include <string>

class Student {
public:
    std::string name;
    std::string id;
    int score;
};

int main() {
    Student s;

    std::getline(std::cin, s.name);
    std::getline(std::cin, s.id);
    std::cin >> s.score;

    std::cout << "Name: " << s.name
              << ", ID: " << s.id
              << ", Score: " << s.score;

    return 0;
}
