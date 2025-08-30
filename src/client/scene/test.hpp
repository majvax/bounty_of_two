#pragma once

#include "base.hpp"
#include "gamestate.hpp"


class TestScene : public SceneABC
{
private:
    gamestate_t state{};

public:
    explicit TestScene(Engine& eng, Client* cli) : SceneABC(eng, cli) {};
    TestScene(const TestScene&) = delete;
    TestScene& operator=(const TestScene&) = delete;
    TestScene(TestScene&&) = delete;
    TestScene& operator=(TestScene&&) = delete;

    ~TestScene() override = default;

    void update(float deltaTime) override;
    void render(sf::RenderTarget& target) override;
    void handleEvent(const sf::Event& event) override;
    void render_menu() override;
    void init() override {};
};
