#pragma once

#include <tuple>

#include "template_helpers.h"

namespace tuple_helpers {
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
} // namespace tuple_helpers
