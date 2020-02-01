#pragma once

#include <tuple>
#include <functional>
#include <type_traits>

#include "tuple_helpers.h"
#include "tuple_of_comparators.h"

namespace implementation {
    // Compile-time resursion exit condition
    template<size_t i, typename Left, typename Right, typename Fns>
    constexpr std::enable_if_t<i == 0, bool>
    compare_tuples(Left&&, Right&&, Fns&&)
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
} // namespace implementation

template<typename Left, typename Right, typename Fns>
constexpr std::enable_if_t<tuple_helpers::are_tuples_v<Left, Right, Fns>, bool>
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
