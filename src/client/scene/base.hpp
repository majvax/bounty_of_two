#pragma once
#include "engine/engine.hpp"
#include "network/client.hpp"
#include <SFML/Graphics.hpp>


class SceneABC
{
protected:
    Engine& engine;
    Client* client{ nullptr };

public:
    explicit SceneABC(Engine& engine, Client* client) : engine(engine), client(client) {};
    SceneABC(const SceneABC&) = delete;
    SceneABC& operator=(const SceneABC&) = delete;
    SceneABC(SceneABC&&) = delete;
    SceneABC& operator=(SceneABC&&) = delete;

    virtual ~SceneABC() = default;
    virtual void init() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderTarget& target) = 0;
    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void render_menu() = 0;
};
