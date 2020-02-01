#pragma once

#include <tuple>
#include <type_traits>
#include <utility>
#include <functional>

#include "template_helpers.h"

namespace implementation {
    // Compile-time recursion
    template<size_t i, typename Tuple, typename ... ResultArgs>
    struct tuple_types
    {
    private:
        using type_raw = std::tuple_element_t<std::tuple_size_v<Tuple> - i, Tuple>;
        using type = std::remove_reference_t<type_raw>;
        using fn_type = std::function<bool(const type&, const type&)>;
        using descent = tuple_types<i - 1, Tuple, ResultArgs ..., fn_type>;

    public:
        using result = typename descent::result;
    };

    // Compile-time resursion exit condition
    template<typename Tuple, typename ... ResultArgs>
    struct tuple_types<0, Tuple, ResultArgs ...>
    {
        using result = std::tuple<ResultArgs ...>;
    };
} // namespace implementation

template<typename Tuple>
struct tuple_types
{
    static_assert (template_helpers::is_specialization_v<Tuple, std::tuple>);
    using type = typename implementation::tuple_types<std::tuple_size_v<Tuple>, Tuple>::result;
};

template<typename Tuple>
using tuple_types_t = typename tuple_types<Tuple>::type;
