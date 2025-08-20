#pragma once
#include <type_traits>
#include <utility>
#include <variant>



namespace details::visitor {
template<class... Callables>
struct Overload : Callables...
{
    using Callables::operator()...;
};



template <typename Variant, typename... Extra>  
struct check_helper {  
    template <typename... Callables>  
    static consteval void check() {  
        []<std::size_t... Is>(std::index_sequence<Is...>) consteval {  
            ( []() consteval {  
                using Alt = std::variant_alternative_t<Is, Variant>;  
                static_assert(  
                    (std::size_t{0} + ... + std::size_t{std::is_invocable_v<Callables, Alt&, Extra&...>}) == 1,  
                    "Each variant alternative must be handled by exactly one callable with (Alt&, Extra&...) (no duplicates or catch-alls).");  
              }(), ...);  
        }(std::make_index_sequence<std::variant_size_v<Variant>>{});  
    }  
};

}


template <typename Variant, typename... Extra, typename... Callables>  
constexpr auto make_unique_visitor(Callables&&... callables) {  
    details::visitor::check_helper<Variant, Extra...>::template check<Callables...>();
    return details::visitor::Overload<std::decay_t<Callables>...>{std::forward<Callables>(callables)...};  
}


template <typename Visitor, typename Variant, typename... Extra>  
constexpr decltype(auto) visit_ctx(Visitor&& visitor, Variant&& var, Extra&&... extra) {  
    return std::visit(  
        [&visitor, &...extra = std::forward<Extra>(extra)](auto&& alt) -> decltype(auto) {  
            return std::forward<Visitor>(visitor)(std::forward<decltype(alt)>(alt), extra...);  
        },  
        std::forward<Variant>(var));  
}


template <typename... Callables>  
constexpr auto make_visitor(Callables&&... callables) {
    return details::visitor::Overload<std::decay_t<Callables>...>{std::forward<Callables>(callables)...};
}
