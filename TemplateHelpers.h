#pragma once

#include <type_traits>

namespace TemplateHelpers {
    // usage:
    // static_assert(is_specialization<std::vector<int>, std::vector>());
    template <class T, template <class...> class Template>
    struct is_specialization : std::false_type {};

    template <template <class...> class Template, class... Args>
    struct is_specialization<Template<Args...>, Template> : std::true_type {};

    // usage:
    // static_assert(is_specialization_v<std::vector<int>, std::vector>);
    template<typename T, template <class...> class Template>
    inline constexpr bool is_specialization_v = is_specialization<std::remove_reference_t<T>, Template>::value;
}
