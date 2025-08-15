#pragma once
#include <SFML/Graphics.hpp>

class SceneABC
{
protected:
    sf::Window& window;

public:
    explicit SceneABC(sf::Window& window) : window(window) {};
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
