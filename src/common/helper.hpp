#include <cmath>
#include <SFML/System/Vector2.hpp>



constexpr void normalize(sf::Vector2f& vec)
{
    const float mag_sq = (vec.x * vec.x) + (vec.y * vec.y);
    if (mag_sq > 0.F) {
        const float mag = std::sqrt(mag_sq);
        vec.x /= mag;
        vec.y /= mag;
    }
}
