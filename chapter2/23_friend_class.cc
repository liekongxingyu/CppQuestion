#include <iostream>
#include <string>

class Secret
{
private:
    std::string data;
    friend class Hacker;

public:
    Secret(std::string s) : data(s) {}
};

class Hacker
{
public:
    void print_data(const Secret &s)
    {
        std::cout << s.data;
    }
};

int main()
{
    Secret s("hacker!");
    Hacker h;
    h.print_data(s);

    return 0;
}
