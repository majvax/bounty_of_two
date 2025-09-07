#pragma once
#include "SFML/Graphics/Rect.hpp"
#include <SFML/Network/Packet.hpp>
#include <SFML/System.hpp>
#include <cstdint>
#include <half.hpp>
#include <variant>


using float16_t = half_float::half;


inline sf::Vector2<float16_t> to_vh(const sf::Vector2f& vec) { return { float16_t{ vec.x }, float16_t{ vec.y } }; }
inline sf::Vector2f to_vf(const sf::Vector2<float16_t>& vec)
{
    return { static_cast<float>(vec.x), static_cast<float>(vec.y) };
}
inline float16_t to_float16(float value) { return float16_t{ value }; }
inline float to_float(float16_t value) { return static_cast<float>(value); }

inline sf::Packet& operator<<(sf::Packet& packet, const float16_t& value)
{
    return packet << std::bit_cast<uint16_t>(value);
}
inline sf::Packet& operator>>(sf::Packet& packet, float16_t& value)
{
    uint16_t data = 0;
    packet >> data;
    value = std::bit_cast<float16_t>(data);
    return packet;
}


inline sf::Packet& operator<<(sf::Packet& packet, const sf::Vector2<float16_t>& vec)
{
    return packet << vec.x << vec.y;
}
inline sf::Packet& operator>>(sf::Packet& packet, sf::Vector2<float16_t>& vec)
{
    return packet >> vec.x >> vec.y;
}


inline sf::Packet& operator<<(sf::Packet& packet, const sf::Vector2f& vec) { return packet << to_vh(vec); }
inline sf::Packet& operator>>(sf::Packet& packet, sf::Vector2f& vec)
{
    sf::Vector2<float16_t> vector_h;
    packet >> vector_h;
    vec = to_vf(vector_h);
    return packet;
}


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
    uint8_t id{ 0 };
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
        // Vector2f automatically convects to half-float
        packet << data.position;
        packet << data.velocity;
        packet << data.acceleration;
        packet << data.size;

        packet << to_float16(data.atq_range);
        packet << to_float16(data.atq_speed);
        packet << to_float16(data.atq_lifetime);
        packet << to_float16(data.reload_time);
        packet << to_float16(data.scale);

        return packet << data.health << data.damage << data.ammo << data.z_index << data.id << data.flags;
    }

    friend sf::Packet& operator>>(sf::Packet& packet, base_t& data)
    {
        packet >> data.position;
        packet >> data.velocity;
        packet >> data.acceleration;
        packet >> data.size;

        float16_t atq_range_h{ 0 };
        float16_t atq_speed_h{ 0 };
        float16_t atq_lifetime_h{ 0 };
        float16_t reload_time_h{ 0 };
        float16_t scale_h{ 0 };
        packet >> atq_range_h >> atq_speed_h >> atq_lifetime_h >> reload_time_h >> scale_h;
        data.atq_range = to_float(atq_range_h);
        data.atq_speed = to_float(atq_speed_h);
        data.atq_lifetime = to_float(atq_lifetime_h);
        data.reload_time = to_float(reload_time_h);
        data.scale = to_float(scale_h);


        return packet >> data.health >> data.damage >> data.ammo >> data.z_index >> data.id >> data.flags;
    }
};
} // namespace details::entity


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
