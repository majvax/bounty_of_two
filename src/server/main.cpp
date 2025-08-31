#include "networking/server.hpp"
#include "visitor.hpp"
#include <random>
#include <ranges>
#include <vector>

constexpr auto SERVER_PORT = 54000;
constexpr int SEND_INTERVAL_MS = 50;

constexpr auto init_visitor = make_visitor([](auto& ent) {
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_real_distribution<float> x_dist(0.F, 1920.F);
    static std::uniform_real_distribution<float> y_dist(0.F, 1080.F);
    static std::uniform_real_distribution<float> vel_dist(-50.F, 50.F);

    ent.position = { x_dist(rng), y_dist(rng) };
    ent.size = { 20.F, 20.F };
    ent.health = 100;
    ent.damage = 10;
    ent.velocity = { vel_dist(rng), vel_dist(rng) };
});

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
    sf::Clock send_clock; // throttle sends

    constexpr int num_entities = 1;
    state.entities.reserve(num_entities);
    for (auto index : std::views::iota(0, num_entities)) {
        if (index % 3 == 0) {
            state.entities.emplace_back(berserker_t{});
        } else if (index % 3 == 1) {
            state.entities.emplace_back(sniper_t{});
        } else {
            state.entities.emplace_back(player_t{});
        }
    }

    for (auto& entity : state.entities) { std::visit(init_visitor, entity); }

    while (true) {
        server.recv();
        
        state.update(clock.restart().asSeconds());

        if (send_clock.getElapsedTime().asMilliseconds() >= SEND_INTERVAL_MS) {
            server.send(state);
            send_clock.restart();
        }
    }

    return 0;
}
