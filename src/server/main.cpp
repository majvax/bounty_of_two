#include "SFML/Network/IpAddress.hpp"
#include "networking/server.hpp"
#include "logger.hpp"
#include <fmt/base.h>

constexpr auto SERVER_PORT = 54000;
constexpr int SEND_INTERVAL_MS = 50;

int main()
{
    init_loggers();

    spdlog::set_level(spdlog::level::info);

    Server server;
    if (auto err = server.init()) {
        spdlog::get("app")->critical("Server initialization failed: {}", *err);
        return -1;
    }
    spdlog::get("app")->info("Server started, waiting for data...");
    fmt::println("Listening on : {}", sf::IpAddress::getPublicAddress()->toString());

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
