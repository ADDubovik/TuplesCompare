#pragma once

#include <tuple>
#include <functional>
#include <type_traits>

#include "template_helpers.h"
#include "tuple_of_comparators.h"

namespace implementation {
    // Compile-time resursion exit condition
    template<size_t i, typename Left, typename Right, typename Fns>
    constexpr std::enable_if_t<i == 0, bool>
    compare_tuples(Left&& left, Right&& right, Fns&& fns)
    {
        return false;
    }

    // Compile-time recursive function
    template<size_t i, typename Left, typename Right, typename Fns>
    constexpr std::enable_if_t<i != 0, bool>
    compare_tuples(Left&& left, Right&& right, Fns&& fns)
    {
        constexpr auto size = std::tuple_size_v<std::remove_reference_t<Fns>>;
        constexpr auto index = size - i;
        auto fn = std::get<index>(fns);
        if (fn(std::get<index>(left), std::get<index>(right))) {
            return true;
        }
        if (fn(std::get<index>(right), std::get<index>(left))) {
            return false;
        }
        return compare_tuples<i - 1>(std::forward<Left>(left), std::forward<Right>(right), std::forward<Fns>(fns));
    }

    template<typename T>
    using is_tuple = template_helpers::is_specialization<T, std::tuple>;

    template<typename T>
    inline constexpr bool is_tuple_v = is_tuple<T>::value;

    template<typename ... Args>
    struct are_tuples;

    template<typename First, typename ... Args>
    struct are_tuples<First, Args ...> : std::conditional_t<is_tuple_v<First>, are_tuples<Args ...>, std::false_type> {};

    template<>
    struct are_tuples<> : std::true_type {};

    template<typename ... Args>
    inline constexpr bool are_tuples_v = are_tuples<Args ...>::value;
} // namespace implementation

template<typename Left, typename Right, typename Fns>
constexpr std::enable_if_t<implementation::are_tuples_v<Left, Right, Fns>, bool>
compare_tuples(Left&& left, Right&& right, Fns&& fns)
{
    using LeftClear = std::remove_reference_t<Left>;
    using RightClear = std::remove_reference_t<Right>;
    using FnsClear = std::remove_reference_t<Fns>;

    static_assert(std::is_same_v<LeftClear, RightClear>);
    static_assert(std::is_same_v<FnsClear, tuple_of_comparators_t<LeftClear>>);

    constexpr auto leftSize = std::tuple_size_v<LeftClear>;
    constexpr auto rightSize = std::tuple_size_v<RightClear>;
    constexpr auto fnsSize = std::tuple_size_v<FnsClear>;

    static_assert(leftSize == rightSize);
    static_assert(leftSize == fnsSize);

    return implementation::compare_tuples<leftSize>(std::forward<Left>(left), std::forward<Right>(right), std::forward<Fns>(fns));
}
