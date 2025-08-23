#pragma once
#include <SFML/Network.hpp>
#include <spdlog/spdlog.h>
#include "networking.hpp"

constexpr auto SERVER_PORT = 54000;

class Client
{
    sf::UdpSocket socket;
    sf::IpAddress server_address = sf::IpAddress::LocalHost;
    uint16_t server_port{ SERVER_PORT };


public:
    Client()
    {
        socket.setBlocking(false);
    }


    bool connect()
    {
        constexpr auto SELF_PORT = 10000;
        spdlog::info("Client connecting to server at {}:{}", server_address.toInteger(), server_port);
        if (socket.bind(SELF_PORT) != sf::Socket::Status::Done) {
            spdlog::critical("Failed to bind socket to port {}", SELF_PORT);
            return false;
        }
        net::packet_t<net::message_type::JoinRequest> packet;
        if (socket.send(packet, server_address, server_port) != sf::Socket::Status::Done) {
            spdlog::error("Failed to send JoinRequest to server at {}:{}", server_address.toInteger(), server_port);
            return false;
        }
        spdlog::info("JoinRequest sent to server at {}:{}", server_address.toInteger(), server_port);
        return true;
    }
    void disconnect()
    {
        socket.unbind();
        spdlog::info("Client disconnected from server");
    }

    template <net::message_type Msg>
    void send(const net::packet_t<Msg>& packet)
    {
        if (socket.send(packet, server_address, server_port) != sf::Socket::Status::Done) {
            spdlog::error("Failed to send packet to server at {}:{}", server_address.toInteger(), server_port);
        }
    }

    template <typename Func>
    void receive(Func&& handler)
    {
        std::optional<sf::IpAddress> sender;
        uint16_t port{ 0 };
        net::packet_t packet{};
        if (socket.receive(packet, sender, port) != sf::Socket::Status::Done) {
            return;
        }

        spdlog::info("Received {} bytes from {}:{}", packet.getDataSize(), sender ? sender->toInteger() : 0, port);
        if (sender && *sender != server_address && port != server_port) {
            spdlog::warn("Received packet from unknown sender {}:{}", sender ? sender->toInteger() : 0, port);
            return;
        }


        net::header_t header;
        if (!(packet >> header)) {
            spdlog::error("Failed to extract header from packet");
            return;
        }
        spdlog::info("Received packet of type {} from server", static_cast<uint8_t>(header.type));
        std::forward<Func>(handler)(header.type, packet);

    }
};
