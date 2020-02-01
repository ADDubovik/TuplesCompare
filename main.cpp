#include <iostream>
#include <string>
#include <vector>

#include "template_helpers.h"
#include "for_each_in_tuple.h"
#include "compare_tuples.h"
#include "tuple_of_comparators.h"

struct A
{
    int data;
    A(int data_) : data(data_) {}
    void print() const {std::cout << "A::data: " << data << std::endl;}
    bool operator<(const A& oth) const {return data < oth.data;}
    bool operator>(const A& oth) const {return data > oth.data;}
};

struct B
{
    std::string data;
    B(std::string data_) : data(std::move(data_)) {}
    void print() const {std::cout << "B::data: " << data << std::endl;}
    bool operator<(const B& oth) const {return data < oth.data;}
    bool operator>(const B& oth) const {return data > oth.data;}
};

struct C
{
    double data;
    C(double data_) : data(data_) {}
    void print() const {std::cout << "C::data: " << data << std::endl;}
    bool operator<(const C& oth) const {return data < oth.data;}
    bool operator>(const C& oth) const {return data > oth.data;}
};

int main()
{
    auto a = A(42);
    auto b = B("Some data");
    auto c = C(3.1415926);
    for_each_in_tuple(std::tie(a, b, c), [](const auto& elem){elem.print();});

    auto tuple = std::tie(a, b, c);
    for_each_in_tuple(tuple, [](const auto& elem){elem.print();});

    using FnA = std::function<bool(const A&, const A&)>;
    using FnB = std::function<bool(const B&, const B&)>;
    using FnC = std::function<bool(const C&, const C&)>;

    std::cout << typeid(FnA).name() << std::endl;
    std::cout << typeid(FnB).name() << std::endl;
    std::cout << typeid(FnC).name() << std::endl;

    std::cout << typeid(std::tuple<FnA, FnB, FnC>).name() << std::endl;

    using Fns = tuple_of_comparators_t<decltype(tuple)>;

    std::cout << typeid(Fns).name() << std::endl;

    auto fns = Fns(std::less(), std::greater(), std::less());

    compare_tuples(tuple, std::tie(a, b, c), fns);

    std::tuple<std::less<int>, std::greater<double>> test;

    return 0;
}
