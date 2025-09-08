#include "player.hpp"
#include "server.hpp"
#include "variant.hpp"
#include <algorithm>
#include <ranges>
#include <spdlog/spdlog.h>

namespace callbacks {
void join_notification(Server* server, callback_params_t callback_params)
{
    auto [packet, conn, state] = *callback_params; // NOLINT
    auto& connections = server->get_connections();

    spdlog::info("JoinNotification message received from {}:{}", conn.get_address().toInteger(), conn.get_port());
    auto iterator = std::ranges::find(connections, conn);

    if (iterator == connections.end() || iterator->authenticated) {
        spdlog::warn(
          "Connection not found or already authenticated: {}:{}", conn.get_address().toInteger(), conn.get_port());
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


    // assign an id to the connection
    static uint8_t next_id = 0;
    iterator->id = ++next_id;
    spdlog::info("Assigned ID {} to connection {}:{}", iterator->id, conn.get_address().toInteger(), conn.get_port());
    // add a player entity to the game state
    player_t player;
    player.id = next_id;
    player.position = { 100.F, 100.F };
    player.size = { 20.F, 20.F };
    player.health = 100;
    player.damage = 10;
    player.velocity = { 0.F, 0.F };
    state.entities.emplace_back(player);
    spdlog::info("Added player entity with ID {} to game state", player.z_index);
}

void leave_notification(Server* server, callback_params_t callback_params)
{
    // TODO: Instead of removing the player, mark them as disconnected and remove after a timeout
    auto [packet, conn, state] = *callback_params; // NOLINT

    if (!conn) {
        spdlog::warn("Invalid connection tried to send LeaveNotification");
        return;
    }

    const auto& connections = server->get_connections();


    spdlog::info("LeaveNotification message received from {}:{}", conn.get_address().toInteger(), conn.get_port());
    auto iterator = std::ranges::find(connections, conn);


    if (iterator == connections.end()) {
        spdlog::warn("Connection not found: {}:{}", conn.get_address().toInteger(), conn.get_port());
        return;
    }

    // remove player from the game state
    auto entities_it = std::ranges::find_if(state.entities, [&conn](const auto& ent) {
        if (auto player = std::get_if<player_t>(&ent)) { return player->id == conn.id; }
        return false;
    });

    if (entities_it == state.entities.end()) {
        spdlog::warn("Player entity not found for connection ID {}", conn.id);
        server->remove_connection(conn.socket, iterator);
        return;
    }

    auto& entity = std::get<player_t>(*entities_it);
    spdlog::info("Removed player entity with ID {} from game state", entity.id);
    state.entities.erase(entities_it);


    spdlog::info("Connection removed: {}:{}", conn.get_address().toInteger(), conn.get_port());
    server->remove_connection(conn.socket, iterator);
}

void chat_message(Server* server, callback_params_t callback_params)
{
    auto [packet, conn, _] = *callback_params; // NOLINT

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

    for (auto& connection : server->get_connections()) {
        // filter invalid connection and sender
        if (connection == conn || !connection) { continue; }
        if (connection.socket.send(out_packet) != sf::Socket::Status::Done) {
            spdlog::error(
              "Failed to send chat message to {}:{}", connection.get_address().toInteger(), connection.get_port());
        }
    }
}

void player_input(Server* /*server*/, callback_params_t callback_params)
{
    auto [packet, conn, state] = *callback_params;
    if (!conn) {
        spdlog::warn("Invalid connection tried to send PlayerInput");
        return;
    }

    net::input_packet_t input_packet;
    if (!(packet >> input_packet)) {
        spdlog::error("Failed to extract input packet from packet");
        return;
    }


    auto players = state.entities | variant::alt_view_v<player_t>;
    auto player_it = std::ranges::find_if(players, [&conn](const player_t& player) { return player.id == conn.id; });
    if (player_it != players.end()) {
        auto& player = *player_it;
        update_velocity(player, input_packet);
    } else {
        spdlog::warn("Player entity not found for connection ID {}", conn.id);
    }
}

} // namespace callbacks
