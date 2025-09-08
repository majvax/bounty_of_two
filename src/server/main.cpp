#include "networking/server.hpp"

constexpr auto SERVER_PORT = 54000;
constexpr int SEND_INTERVAL_MS = 50;

int main()
{
    spdlog::set_level(spdlog::level::info);

    Server server;
    if (auto err = server.init()) {
        spdlog::critical("Server initialization failed: {}", *err);
        return -1;
    }
    spdlog::info("Server started, waiting for data...");

    sf::Clock clock;
    gamestate_t state;
    sf::Clock send_clock;

    while (true) {
        server.recv(state);

        state.update(clock.restart().asSeconds());

        if (send_clock.getElapsedTime().asMilliseconds() >= SEND_INTERVAL_MS) {
            server.send(state);
            send_clock.restart();
        }
    }

    return 0;
}
