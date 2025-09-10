#include "engine/engine.hpp"
#include "network/client.hpp"
#include "scene/home.hpp"
#include "logger.hpp"

int main()
{
    init_loggers();
    spdlog::set_level(spdlog::level::info);

    Engine engine;
    Client client;
    engine.pushScene(std::make_unique<HomeScene>(engine, &client));
    engine.run();
}
