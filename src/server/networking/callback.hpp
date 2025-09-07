#pragma once
#include "connection.hpp"
#include "networking.hpp"


class Server;

struct callback_params_t
{
    net::packet_t<>* packet{ nullptr };
    connection_t* conn{ nullptr };
    gamestate_t* state{ nullptr };

    [[nodiscard]] auto operator*() const { return std::tie(*packet, *conn, *state); }
};


namespace callbacks {
void join_notification(Server* server, callback_params_t callback_params);
void leave_notification(Server* server, callback_params_t callback_params);
void chat_message(Server* server, callback_params_t callback_params);
void player_input(Server* server, callback_params_t callback_params);
} // namespace callbacks
