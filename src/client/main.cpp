#include "engine/engine.hpp"
#include "network/client.hpp"
#include "scene/cube.hpp"
#include "scene/title.hpp"
#include "logger.hpp"

int main()
{
    init_loggers();
    spdlog::set_level(spdlog::level::info);

    Engine engine;
    Client client;
    if (!client.connect()) {
        spdlog::critical("Failed to connect to server, exiting");
        return -1;
    }

    engine.pushScene(std::make_unique<TitleScene>(engine, &client));
    engine.pushScene(std::make_unique<SceneCube>(engine, &client));

    engine.run();
}
