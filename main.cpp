#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

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

struct Car
{
    Car(std::string manufacturer_, std::string model_, uint32_t motor_)
        : manufacturer(std::move(manufacturer_))
        , model(std::move(model_))
        , motor(motor_)
    {}

    std::string manufacturer;
    std::string model;
    uint32_t motor;

    using Rank = std::tuple<const std::string&, const std::string&, const uint32_t&>;
    Rank rank() const {return std::tie(manufacturer, model, motor);}
};

std::ostream& operator<<(std::ostream& stream, const Car& car)
{
    return stream << car.manufacturer << " " << car.model << " " << car.motor;
}

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

    std::vector<Car> cars {{"Toyota", "Prius", 1800},
                           {"Toyota", "Highlander", 3500},
                           {"Toyota", "Highlander", 3000},
                           {"Toyota", "Landcruiser", 3000},
                           {"Toyota", "Landcruiser", 3500},
                           {"Toyota", "Landcruiser", 4200},
                           {"Mercedes", "C-classe", 1800},
                           {"Mercedes", "C-classe", 2200},
                           {"Mercedes", "C-classe", 3500},
                           {"Mercedes", "ML", 3000},
                           {"Mercedes", "ML", 3500},
                           {"Mercedes", "ML", 4200},
                           {"Audi", "A4", 1800},
                           {"Audi", "A4", 2000},
                           {"Audi", "A4", 2200},
                           {"Audi", "A6", 2200},
                           {"Audi", "A6", 3000},
                           {"Audi", "A6", 3500},
                           {"Audi", "A6", 4000},
                           {"Audi", "A8", 3000},
                           {"Audi", "A8", 3500},
                           {"Audi", "A8", 4000},
                           {"Volkswagen", "Passat", 1800},
                           {"Volkswagen", "Passat", 2000},
                           {"Volkswagen", "Passat", 2200},
                           {"Volkswagen", "Passat", 2500},
                           {"Volkswagen", "Passat", 2800},
                           {"Volkswagen", "Golf", 1500},
                           {"Volkswagen", "Golf", 1800},
                           {"Volkswagen", "Golf", 2200},
                           {"Volkswagen", "Touareg", 2500},
                           {"Volkswagen", "Touareg", 2800},
                           {"Volkswagen", "Touareg", 3000},
                           {"Volkswagen", "Touareg", 3500},
                          };

    auto comparators = tuple_of_comparators_t<Car::Rank>(std::less(), std::greater(), std::greater());
    std::sort(cars.begin(), cars.end(), [&comparators](const auto& left, const auto& right){return compare_tuples(left.rank(), right.rank(), comparators);});
    for (auto const &elem : cars) {
        std::cout << elem << std::endl;
    }

    return 0;
}
