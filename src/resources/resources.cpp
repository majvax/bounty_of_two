#include "resources.hpp"

#include <array>
#include <bit>
#include <cstdint>

static constexpr auto hover = [] {
    // clang-format off
    // NOLINTNEXTLINE
    constexpr std::uint8_t tmp[] = {
        #embed "assets/sound.wav" // NOLINT
    };
    // clang-format on
    constexpr auto arr = std::to_array(tmp);
    return std::bit_cast<std::array<std::byte, arr.size()>>(arr);
}();

static constexpr auto nunito = [] {
    // clang-format off
    // NOLINTNEXTLINE
    constexpr std::uint8_t tmp[] = {
        #embed "assets/Nunito-Regular.ttf" // NOLINT
    };
    // clang-format on
    constexpr auto arr = std::to_array(tmp);
    return std::bit_cast<std::array<std::byte, arr.size()>>(arr);
}();


namespace resources {
std::span<const std::byte> get_hover() { return hover; }
// NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
std::span<std::byte> legacy_get_hover() { return { const_cast<std::byte*>(hover.data()), hover.size() }; }

std::span<const std::byte> get_nunito() { return nunito; }
// NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
std::span<std::byte> legacy_get_nunito() { return { const_cast<std::byte*>(nunito.data()), nunito.size() }; }
} // namespace resources
