#include "network.hpp"
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
    std::vector<connection_t> connections;


    sf::UdpSocket socket;
    socket.setBlocking(false);


    if (socket.bind(SERVER_PORT) != sf::Socket::Status::Done) {
        spdlog::error("Failed to bind socket on port 54000");
        return -1;
    }
    spdlog::info("Server started, waiting for data...");

    sf::Clock clock;
    gamestate_t state;
    sf::Clock send_clock; // throttle sends


    constexpr int num_entities = 1000;
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
        const auto delta_time = clock.restart().asSeconds();
        recv_and_process(socket, connections);
        state.update(delta_time);

        if (send_clock.getElapsedTime().asMilliseconds() >= SEND_INTERVAL_MS) {
            for (const auto& conn : connections) { send(socket, state, conn); }
            send_clock.restart();
        }
    }

    return 0;
}
