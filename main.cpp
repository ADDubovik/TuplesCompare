#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "compare_tuples.h"

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

    auto comparators = tuple_of_comparators_t<Car::Rank>(std::less(), std::less(), std::greater());
    std::sort(cars.begin(), cars.end(), [&comparators](const auto& left, const auto& right){return compare_tuples(left.rank(), right.rank(), comparators);});
    for (auto const &elem : cars) {
        std::cout << elem << std::endl;
    }

    return 0;
}
