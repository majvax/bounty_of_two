#pragma once
#include "callback.hpp"
#include "connection.hpp"
#include "gamestate.hpp"
#include "networking.hpp"
#include <SFML/Network.hpp>


class Server
{
    std::vector<connection_t> m_connections;
    std::map<net::message_type, std::function<void(Server*, callback_params_t)>> m_message_handlers;

    sf::UdpSocket m_data_socket;
    sf::TcpListener m_listener;
    sf::SocketSelector m_selector;

    void accept_new_connections();
    void process_tcp_message(connection_t& conn, gamestate_t& state);
    void process_udp_message(gamestate_t& state);
    void setup_callbacks();

public:
    Server() = default;
    ~Server() = default;
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    Server(Server&&) = delete;
    Server& operator=(Server&&) = delete;

    std::optional<std::string> init();
    void recv(gamestate_t& state);
    void send(const gamestate_t& state);
    [[nodiscard]] const auto& get_connections() const { return m_connections; }
    [[nodiscard]] auto& get_connections() { return m_connections; }
    void remove_connection(sf::TcpSocket& socket, auto& iterator)
    {
        m_selector.remove(socket);
        m_connections.erase(iterator);
    }
};
