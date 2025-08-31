#pragma once
#include "networking.hpp"
#include <SFML/Network.hpp>
#include <cstdint>
#include <functional>
#include <map>
#include <vector>


class Server
{
    struct connection_t
    {
        sf::TcpSocket socket;
        uint32_t id{ 0 };
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

    std::vector<connection_t> m_connections;
    std::map<net::message_type, std::function<void(net::packet_t<>&, connection_t&)>> m_message_handlers;

    sf::UdpSocket m_data_socket;
    sf::TcpListener m_listener;
    sf::SocketSelector m_selector;

    void accept_new_connections();
    void process_tcp_message(connection_t& conn);
    void process_udp_message();
    void setup_callbacks();
public:
    Server() = default;
    ~Server() = default;
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    Server(Server&&) = delete;
    Server& operator=(Server&&) = delete;

    std::optional<std::string> init();
    void recv();
    void send(const gamestate_t& state);
};
