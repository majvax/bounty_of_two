#include "server.hpp"
#include <algorithm>


void Server::setup_callbacks()
{
    m_message_handlers[net::message_type::JoinNotification] = [this](auto& packet, auto& conn) {
        spdlog::info("JoinNotification message received from {}:{}", conn.get_address().toInteger(), conn.get_port());
        auto iterator = std::ranges::find(m_connections, conn);

        if (iterator == m_connections.end() || iterator->authenticated) {
            spdlog::warn("Connection not found or already authenticated: {}:{}", conn.get_address().toInteger(), conn.get_port());
            return;
        }

        iterator->authenticated = true;
        spdlog::info("Connection authenticated: {}:{}", conn.get_address().toInteger(), conn.get_port());

        net::join_packet_t join_packet;
        if (!(packet >> join_packet)) {
            spdlog::error("Failed to extract join packet from packet");
            return;
        }
        iterator->udp_port = join_packet.udp_port;
        spdlog::info("Client UDP port set to {}", iterator->udp_port);
    };

    m_message_handlers[net::message_type::LeaveNotification] = [this](auto&, auto& conn) {
        if (!conn) {
            spdlog::warn("Invalid connection tried to send LeaveNotification");
            return;
        }

        spdlog::info("LeaveNotification message received from {}:{}", conn.get_address().toInteger(), conn.get_port());
        auto iterator = std::ranges::find(m_connections, conn);
        if (iterator != m_connections.end()) {
            spdlog::info("Connection removed: {}:{}", conn.get_address().toInteger(), conn.get_port());
            m_selector.remove(conn.socket);
            m_connections.erase(iterator);
        } else {
            spdlog::info("Connection not found: {}:{}", conn.get_address().toInteger(), conn.get_port());
        }
    };

    m_message_handlers[net::message_type::ChatMessage] = [this](auto& packet, auto& conn) {
        if (!conn) {
            spdlog::warn("Invalid connection tried to send ChatMessage");
            return;
        }

        spdlog::info("ChatMessage received from {}:{}", conn.get_address().toInteger(), conn.get_port());
        net::chat_packet_t chat_packet;
        if (!(packet >> chat_packet)) {
            spdlog::error("Failed to extract chat message from packet");
            return;
        }
        spdlog::info("Message from {}: {}", chat_packet.sender_id, chat_packet.message);

        net::packet_t<net::message_type::ChatMessage> out_packet{};
        out_packet << chat_packet;

        for (auto& connection : m_connections) {
            if (connection == conn) { continue; }
            if (connection.socket.send(out_packet) != sf::Socket::Status::Done) {
                spdlog::error(
                  "Failed to send chat message to {}:{}", connection.get_address().toInteger(), connection.get_port());
            }
        }
    };
}
