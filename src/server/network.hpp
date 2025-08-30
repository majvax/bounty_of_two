#include "networking.hpp"
#include <SFML/Network.hpp>
#include <array>
#include <cstddef>
#include <algorithm>

constexpr auto MAX_PACKET_SIZE = 1024;

struct connection_t
{
    sf::IpAddress address{ 0 };
    uint16_t port{ 0 };
    uint16_t id{ 0 };


    bool operator==(const connection_t& other) const
    {
        return address == other.address && port == other.port && id == other.id;
    }
};


inline void recv_and_process(sf::UdpSocket& socket, std::vector<connection_t>& connections)
{
    std::array<std::byte, MAX_PACKET_SIZE> buffer{};


    std::optional<sf::IpAddress> sender;
    uint16_t port{ 0 };

    net::packet_t packet{};

    if (socket.receive(packet, sender, port) != sf::Socket::Status::Done) {
        spdlog::error("Failed to receive data");
        return;
    }

    spdlog::info("Received {} bytes from {}:{}", packet.getDataSize(), sender->toInteger(), port);

    net::header_t header;
    if (!(packet >> header)) {
        spdlog::error("Failed to extract header from packet");
        return;
    }

    spdlog::info("Message type: {}", static_cast<uint8_t>(header.type));

    switch (header.type) {
    case net::message_type::GameUpdate: {
        spdlog::critical("shouldn't happen...");
        break;
    }
    case net::message_type::PlayerInput:
        spdlog::info("PlayerInput message received");
        break;
    case net::message_type::JoinRequest: {
        spdlog::info("JoinRequest message received");
        connection_t new_conn{ .address=*sender, .port=port, .id=static_cast<uint16_t>(connections.size() + 1) };
        if (std::ranges::find(connections, new_conn) == connections.end()) {
            connections.push_back(new_conn);
            spdlog::info("New connection added: {}:{}", new_conn.address.toInteger(), new_conn.port);
        } else {
            spdlog::info("Connection already exists: {}:{}", new_conn.address.toInteger(), new_conn.port);
        }
        break;
    }
    default:
        spdlog::warn("Unknown message type received: {}", static_cast<uint8_t>(header.type));
        break;
    }
    }


    inline void send(sf::UdpSocket & socket, gamestate_t & state, const connection_t& conn)
    {
        net::packet_t<net::message_type::GameUpdate> packet{};

        net::gamestate_packet_t gs_packet;
        gs_packet.from_gamestate(state);

        if (!(packet << gs_packet)) {
            spdlog::error("Failed to insert gamestate into packet");
            return;
        }


        if (socket.send(packet, conn.address, conn.port) != sf::Socket::Status::Done) {
            spdlog::error("Failed to send packet");
            return;
        }

        spdlog::info("Sent {} bytes to {}:{}", packet.getDataSize(), conn.address.toInteger(), conn.port);
    }
