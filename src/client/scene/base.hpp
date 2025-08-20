#pragma once
#include <SFML/Graphics.hpp>
#include "engine/engine.hpp"

class SceneABC
{
protected:
    Engine& engine;

public:
    explicit SceneABC(Engine& engine): engine(engine) {};
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
