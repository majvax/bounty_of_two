#pragma once
#include <SFML/Network.hpp>
#include <cstdint>

struct connection_t
{
    sf::TcpSocket socket;
    uint8_t id{ 0 };
    uint16_t udp_port{ 0 };
    bool valid{ false };
    bool authenticated{ false };


    [[nodiscard]] sf::IpAddress get_address() const
    {
        if (auto addr = socket.getRemoteAddress()) { return *addr; }
        return sf::IpAddress::Any;
    }
    [[nodiscard]] uint16_t get_port() const { return socket.getRemotePort(); }

    bool operator==(const connection_t& other) const
    {
        return get_address() == other.get_address() && get_port() == other.get_port() && id == other.id;
    }
    explicit operator bool() const { return valid && authenticated && socket.getRemoteAddress(); }
};
