#include <iostream>
#include <string>
#include <memory>

class Printer {
public:
    virtual void print() const = 0;
    virtual ~Printer() {}
};

class TextPrinter : public Printer {
private:
    std::string text;
public:
    TextPrinter(const std::string& t) : text(t) {}
    void print() const override {
        std::cout << text;
    }
};

class NumberPrinter : public Printer {
private:
    int number;
public:
    NumberPrinter(int n) : number(n) {}
    void print() const override {
        std::cout << number;
    }
};

int main() {
    std::string type;
    std::cin >> type;

    std::unique_ptr<Printer> p = nullptr;

    if (type == "text") {
        std::string s;
        std::cin.ignore();
        std::getline(std::cin, s);
        p = std::unique_ptr<Printer>(new TextPrinter(s));
    } else if (type == "number") {
        int n;
        std::cin >> n;
        p = std::unique_ptr<Printer>(new NumberPrinter(n));
    }

    p->print();
    return 0;
}
