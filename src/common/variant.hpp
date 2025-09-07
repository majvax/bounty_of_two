#pragma once
#include <ranges>
#include <type_traits>
#include <utility>
#include <variant>


namespace variant::details {

template<typename T, typename Variant>
constexpr bool is_in_variant_v = []<std::size_t... I>(std::index_sequence<I...>) {
    return (std::is_same_v<T, std::variant_alternative_t<I, Variant>> || ...);
}(std::make_index_sequence<std::variant_size_v<Variant>>{});


template<typename T>
struct get_variant_of_type_t : std::ranges::range_adaptor_closure<get_variant_of_type_t<T>>
{
    template<std::ranges::viewable_range Range>
    auto operator()(Range&& range) const
    {
        using Variant = std::remove_reference_t<decltype(*std::begin(range))>;
        static_assert(is_in_variant_v<T, Variant>, "Type T must be a member of the variant");
        return std::forward<Range>(range)
               | std::views::filter([](Variant& variant) { return std::holds_alternative<T>(variant); })
               | std::views::transform([](Variant& variant) -> T& { return std::get<T>(variant); });
    }
};

} // namespace variant::details

namespace variant {
template<typename T>
inline constexpr variant::details::get_variant_of_type_t<T> alt_view_v{};
} // namespace variant
