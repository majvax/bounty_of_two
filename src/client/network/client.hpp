#pragma once
#include "networking.hpp"
#include <SFML/Network.hpp>
#include <spdlog/spdlog.h>

constexpr auto TCP_SERVER_PORT = 54000;
constexpr auto UDP_SERVER_PORT = 54001;

constexpr auto UDP_SELF_PORT = 10000;


enum struct receive_status : uint8_t { WouldBlock, UnknownSender, Error };

class Client
{
    sf::TcpSocket tcp_socket;
    sf::UdpSocket udp_socket;
    sf::IpAddress server_address = sf::IpAddress::LocalHost;


public:
    Client()
    {
        udp_socket.setBlocking(false);
    }
    ~Client() { disconnect(); }
    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;
    Client(Client&&) = delete;
    Client& operator=(Client&&) = delete;


    bool connect()
    {

        auto status = tcp_socket.connect(server_address, TCP_SERVER_PORT);
        if (status != sf::Socket::Status::Done)
        {
            spdlog::critical("Failed to connect to server at {}:{}", server_address.toInteger(), TCP_SERVER_PORT);
            return false;
        }
        tcp_socket.setBlocking(false);


        status = udp_socket.bind(UDP_SELF_PORT);
        if (status != sf::Socket::Status::Done)
        {
            spdlog::critical("Failed to bind UDP socket to port {}", UDP_SELF_PORT);
            return false;
        }

        spdlog::info("Connected to server at {}:{}", server_address.toInteger(), TCP_SERVER_PORT);

        net::packet_t<net::message_type::JoinNotification> packet{};
        packet << net::join_packet_t{.udp_port = UDP_SELF_PORT, .player_name = "Player" };
        status = tcp_socket.send(packet);
        if (status != sf::Socket::Status::Done)
        {
            spdlog::error("Failed to send JoinNotification to server at {}:{}", server_address.toInteger(), TCP_SERVER_PORT);
            return false;
        }

        spdlog::info("JoinRequest sent to server at {}:{}", server_address.toInteger(), TCP_SERVER_PORT);
        return true;
    }
    void disconnect()
    {
        net::packet_t<net::message_type::LeaveNotification> packet{};
        if (tcp_socket.send(packet) != sf::Socket::Status::Done) {
            spdlog::error("Failed to send LeaveNotification to server at {}:{}", server_address.toInteger(), TCP_SERVER_PORT);
        }
        tcp_socket.disconnect();
        udp_socket.unbind();
        spdlog::info("Client disconnected from server");
    }

    /**
     * @brief send a message to the server using TCP
     * 
     * @tparam Msg 
     * @param packet 
     */
    template<net::message_type Msg>
    void send_message(const net::packet_t<Msg>& packet)
    {
        if (tcp_socket.send(packet) != sf::Socket::Status::Done) {
            spdlog::error("Failed to send packet to server at {}:{}", server_address.toInteger(), TCP_SERVER_PORT);
        }
    }

    /**
     * @brief send a message to the server using UDP
     * 
     * @tparam Msg 
     * @param packet 
     */
    template<net::message_type Msg>
    void send_data(net::packet_t<Msg>& packet)
    {
        auto status = udp_socket.send(packet, server_address, UDP_SERVER_PORT);
        if (status != sf::Socket::Status::Done) {
            spdlog::error("Failed to send packet to server at {}:{}", server_address.toInteger(), UDP_SERVER_PORT);
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

        const auto status = udp_socket.receive(packet, sender, port);
        if (status == sf::Socket::Status::NotReady) { return std::optional{ receive_status::WouldBlock }; }

        if (status != sf::Socket::Status::Done) { return std::optional{ receive_status::Error }; }

        spdlog::info("Received {} bytes from {}:{}", packet.getDataSize(), sender ? sender->toInteger() : 0, port);
        if (sender && *sender != server_address && port != UDP_SERVER_PORT) {
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
