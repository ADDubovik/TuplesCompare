#pragma once

#include <tuple>
#include <functional>
#include <type_traits>

#include "template_helpers.h"

namespace implementation {
    // Compile-time resursion exit condition
    template<size_t i, typename Tuple, typename Fn>
    std::enable_if_t<i == 0>
    for_each_in_tuple(Tuple&&, Fn) {}

    // Compile-time recursion function
    template<size_t i, typename Tuple, typename Fn>
    std::enable_if_t<i != 0>
    for_each_in_tuple(Tuple&& tuple, Fn fn)
    {
        for_each_in_tuple<i - 1>(std::forward<Tuple>(tuple), fn);
        fn(std::get<i - 1>(tuple));
    }
} // namespace implementation

template<typename Container, typename Fn>
std::enable_if_t<template_helpers::is_specialization_v<Container, std::tuple>>
for_each_in_tuple(Container&& container, Fn fn)
{
    using Container_t = std::remove_reference_t<Container>;
    constexpr auto size = std::tuple_size_v<Container_t>;
    implementation::for_each_in_tuple<size>(std::forward<Container>(container), fn);
}
