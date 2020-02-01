#include <tuple>
#include <functional>

namespace implementation {
    // Compile-time resursion exit condition
    template<size_t i, typename Fn, typename ... Ts>
    typename std::enable_if_t<i == 0>
    for_each_in_tuple(std::tuple<Ts...>&, Fn) {}

    // Compile-time recursion function
    template<size_t i, typename Fn, typename ... Ts>
    typename std::enable_if_t<i != 0>
    for_each_in_tuple(std::tuple<Ts...>& tuple, Fn fn)
    {
        for_each_in_tuple<i - 1>(tuple, fn);
        fn(std::get<i - 1>(tuple));
    }
} // namespace implementation

template<typename Fn, typename ... Ts>
void for_each_in_tuple(std::tuple<Ts...>&& tuple, Fn fn)
{
    implementation::for_each_in_tuple<sizeof...(Ts)>(tuple, fn);
}
