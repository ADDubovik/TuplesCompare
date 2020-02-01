#include <iostream>
#include <string>

#include "for_each_in_tuple.h"

struct A
{
    int data;
    A(int data_) : data(data_) {}
    void print() const {std::cout << "A::data: " << data << std::endl;}
};

struct B
{
    std::string data;
    B(std::string data_) : data(std::move(data_)) {}
    void print() const {std::cout << "B::data: " << data << std::endl;}
};

int main()
{
    auto a = A(42);
    auto b = B("Some data");
    for_each_in_tuple(std::tie(a, b), [](const auto& elem){elem.print();});

    return 0;
}
