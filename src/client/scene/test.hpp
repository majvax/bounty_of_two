#pragma once

#include "base.hpp"
#include "gamestate.hpp"


class TestScene : public SceneABC
{
private:
    using input_enum_t = std::underlying_type_t<net::input_type>;

    gamestate_t state{};
    static input_enum_t build_input_mask();
    sf::Clock input_clock;

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
