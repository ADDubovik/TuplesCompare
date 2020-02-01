#pragma once

#include <type_traits>

namespace template_helpers {
    // usage:
    // static_assert(is_specialization_strict<std::vector<int>, std::vector>::value);
    template <class T, template <class...> class Template>
    struct is_specialization_strict : std::false_type {};

    template <template <class...> class Template, class... Args>
    struct is_specialization_strict<Template<Args...>, Template> : std::true_type {};

    // usage:
    // static_assert(is_specialization<std::vector<int>, std::vector>::value);
    template<typename T, template <class...> class Template>
    using is_specialization = is_specialization_strict<std::remove_reference_t<T>, Template>;

    // usage:
    // static_assert(is_specialization_v<std::vector<int>, std::vector>);
    template<typename T, template <class...> class Template>
    inline constexpr bool is_specialization_v = is_specialization<T, Template>::value;
}
