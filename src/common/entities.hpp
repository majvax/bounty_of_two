#pragma once
#include "SFML/Graphics/Rect.hpp"
#include "half.hpp"
#include <SFML/Network/Packet.hpp>
#include <SFML/System.hpp>
#include <cstdint>
#include <variant>



using half_float::half;
using Vector2h = sf::Vector2<half>;


enum struct EntityFlags : uint8_t {
    None = 0,
    Damaged = 1U << 0U,
    Attacking = 1U << 1U,
    Reloading = 1U << 2U,
    Dead = 1U << 3U,
    Reserved1 = 1U << 4U,
    Reserved2 = 1U << 5U,
};


namespace details::entity {
struct base_t
{
    // Use std::underlying_type_t to get the underlying type of EntityFlags
    // This allows us to use the enum as a bitmask
    // for now the enum is uint8_t but using EnumBase allows for future changes
    // to the underlying type without changing the code
    using enum_t = std::underlying_type_t<EntityFlags>;

    sf::Vector2f position{ 0.F, 0.F };
    sf::Vector2f velocity{ 0.F, 0.F };
    sf::Vector2f acceleration{ 0.F, 0.F };
    sf::Vector2f size{ 0.F, 0.F };

    // Attack / timing (5 * 4B = 20B) -> offset 32..52
    float atq_range{ 0.F };
    float atq_speed{ 0.F };
    float atq_lifetime{ 0.F };
    float reload_time{ 0.F };
    float scale{ 1.F };

    // Compact stats -> offset 54..60
    uint16_t health{ 0 };
    uint16_t damage{ 0 };
    uint16_t ammo{ 0 };
    uint8_t z_index{ 0 };
    enum_t flags{ 0 };

    [[nodiscard]] sf::FloatRect get_bounds() const { return { position, size }; }

    void add_flags(EntityFlags new_flags = EntityFlags::None)
    {

        const auto mask = static_cast<enum_t>(new_flags);
        flags |= mask;
    }

    void clear_flags(EntityFlags clear_flags = EntityFlags::None)
    {
        const auto mask = static_cast<enum_t>(clear_flags);
        const auto inverted = static_cast<enum_t>(~mask);
        flags &= inverted;
    }

    [[nodiscard]] bool has_flag(EntityFlags check_flags) const
    {
        const auto mask = static_cast<enum_t>(check_flags);
        return (flags & mask) == mask;
    }


    // Maybe we should put the serialization operators in a separate file
    friend sf::Packet& operator<<(sf::Packet& packet, const base_t& data)
    {
        return packet << data.position.x << data.position.y << data.velocity.x << data.velocity.y << data.acceleration.x
                      << data.acceleration.y << data.size.x << data.size.y << data.atq_range << data.atq_speed
                      << data.atq_lifetime << data.reload_time << data.scale << data.health << data.damage << data.ammo
                      << data.z_index << data.flags;
    }

    friend sf::Packet& operator>>(sf::Packet& packet, base_t& data)
    {
        return packet >> data.position.x >> data.position.y >> data.velocity.x >> data.velocity.y >> data.acceleration.x
               >> data.acceleration.y >> data.size.x >> data.size.y >> data.atq_range >> data.atq_speed
               >> data.atq_lifetime >> data.reload_time >> data.scale >> data.health >> data.damage >> data.ammo
               >> data.z_index >> data.flags;
    }
};
}// namespace details::entity


struct player_t : details::entity::base_t
{
};
struct berserker_t : details::entity::base_t
{
};
struct sniper_t : details::entity::base_t
{
};


using entity_t = std::variant<player_t, berserker_t, sniper_t>;
