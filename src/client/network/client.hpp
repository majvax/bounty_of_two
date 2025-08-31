#pragma once
#include "networking.hpp"
#include <SFML/Network.hpp>
#include <spdlog/spdlog.h>

constexpr auto SERVER_PORT = 54000;

enum struct receive_status : uint8_t { WouldBlock, UnknownSender, Error };

class Client
{
    sf::UdpSocket socket;
    sf::IpAddress server_address = sf::IpAddress::LocalHost;
    uint16_t server_port{ SERVER_PORT };


public:
    Client() { socket.setBlocking(false); }


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

    template<net::message_type Msg>
    void send(const net::packet_t<Msg>& packet)
    {
        if (socket.send(packet, server_address, server_port) != sf::Socket::Status::Done) {
            spdlog::error("Failed to send packet to server at {}:{}", server_address.toInteger(), server_port);
        }
    }

    /**
     * @brief
     *
     * @param handler : function to handle received messages
     * @return std::optional<receive_status> : status of the receive operation (WouldBlock, UnknownSender, Error) or
     * std::nullopt on success
     */
    auto receive(const std::function<void(net::message_type, sf::Packet&)>& handler) -> std::optional<receive_status>
    {
        std::optional<sf::IpAddress> sender;
        uint16_t port{ 0 };
        net::packet_t packet{};

        const auto status = socket.receive(packet, sender, port);
        if (status == sf::Socket::Status::NotReady) { return std::optional{ receive_status::WouldBlock }; }

        if (status != sf::Socket::Status::Done) { return std::optional{ receive_status::Error }; }

        spdlog::info("Received {} bytes from {}:{}", packet.getDataSize(), sender ? sender->toInteger() : 0, port);
        if (sender && *sender != server_address && port != server_port) {
            spdlog::warn("Received packet from unknown sender {}:{}", sender ? sender->toInteger() : 0, port);
            return std::optional{ receive_status::UnknownSender };
        }


        net::header_t header;
        if (!(packet >> header)) {
            spdlog::error("Failed to extract header from packet");
            return std::optional{ receive_status::Error };
        }
        spdlog::info("Received packet of type {} from server", static_cast<uint8_t>(header.type));
        handler(header.type, packet);
        return std::nullopt;
    }
};
