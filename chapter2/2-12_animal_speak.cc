#include <iostream>
#include <string>

class Animal
{
public:
    virtual void speak() const = 0;
    virtual ~Animal() {}   // 虚析构
};

class Dog : public Animal
{
public:
    void speak() const override
    {
        std::cout << "woof" << std::endl;
    }
};

class Cat : public Animal
{
public:
    void speak() const override
    {
        std::cout << "meow" << std::endl;
    }
};

int main()
{
    std::string type;
    std::cin >> type;

    Animal* animal = nullptr;

    if (type == "dog")
    {
        animal = new Dog();
    }
    else if (type == "cat")
    {
        animal = new Cat();
    }

    if (animal != nullptr)
    {
        animal->speak();
    }

    delete animal;
    return 0;
}
