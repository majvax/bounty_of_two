#include <SFML/System/Vector2.hpp>
#include <charconv> // from_chars
#include <cmath>
#include <cstdint>
#include <limits>
#include <string_view>


constexpr void normalize(sf::Vector2f& vec)
{
    const float mag_sq = (vec.x * vec.x) + (vec.y * vec.y);
    if (mag_sq > 0.F) {
        const float mag = std::sqrt(mag_sq);
        vec.x /= mag;
        vec.y /= mag;
    }
}


constexpr std::uint8_t to_u8(std::string_view string)
{
    unsigned int value = 0; // parse into wider type
    auto res = std::from_chars(string.begin(), string.end(), value);
    bool success = (res.ec == std::errc{} && res.ptr == string.end() && value <= std::numeric_limits<std::uint8_t>::max());
    return success ? static_cast<std::uint8_t>(value) : 0;
}
