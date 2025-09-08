#include "helper.hpp"
#include "networking.hpp"
#include <SFML/System/Vector2.hpp>


inline void update_velocity(player_t& player, const net::input_packet_t& input_packet)
{
    const float speed = 200.F;
    sf::Vector2f direction{ 0.F, 0.F };

    {
        using underlying_t = std::underlying_type_t<net::input_type>;
        using enum net::input_type;

        if ((input_packet.input & static_cast<underlying_t>(MoveUp)) != 0) { direction.y -= 1.F; }
        if ((input_packet.input & static_cast<underlying_t>(MoveDown)) != 0) { direction.y += 1.F; }
        if ((input_packet.input & static_cast<underlying_t>(MoveLeft)) != 0) { direction.x -= 1.F; }
        if ((input_packet.input & static_cast<underlying_t>(MoveRight)) != 0) { direction.x += 1.F; }
    }

    normalize(direction);
    player.velocity = direction * speed;
}
