#include <iostream>
#include <string>

class Book
{
private:
    std::string title;
    std::string author;

public:
    Book(const std::string &t, const std::string &a)
        : title(t), author(a) {}

    std::string getTitle() const
    {
        return title;
    }

    std::string getAuthor() const
    {
        return author;
    }
};

class Library
{
private:
    Book book; // 组合：Library "拥有" 一个 Book

public:
    Library(const Book &b) : book(b) {}

    void printInfo() const
    {
        std::cout << "Title: " << book.getTitle()
                  << ", Author: " << book.getAuthor();
    }
};

int main()
{
    std::string title;
    std::string author;

    std::getline(std::cin, title);
    std::getline(std::cin, author);

    Book b(title, author);
    Library lib(b);

    lib.printInfo();

    return 0;
}
