#include <iostream>

int main() {
    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1:
            std::cout << "add";
            break;
        case 2:
            std::cout << "delete";
            break;
        case 3:
            std::cout << "update";
            break;
        case 4:
            std::cout << "query";
            break;
        default:
            std::cout << "exit";
            break;
    }

    return 0;
}
