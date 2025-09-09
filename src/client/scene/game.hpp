#pragma once

#include "base.hpp"
#include "gamestate.hpp"
#include <memory>


class GameScene : public SceneABC
{
private:
    using input_enum_t = std::underlying_type_t<net::input_type>;
    std::shared_ptr<spdlog::logger> logger;
    gamestate_t state{};
    static input_enum_t build_input_mask();
    sf::Clock input_clock;

public:
    explicit GameScene(Engine& eng, Client* cli);
    GameScene(const GameScene&) = delete;
    GameScene& operator=(const GameScene&) = delete;
    GameScene(GameScene&&) = delete;
    GameScene& operator=(GameScene&&) = delete;

    ~GameScene() override = default;

    void update(float deltaTime) override;
    void render(sf::RenderTarget& target) override;
    void handleEvent(const sf::Event& event) override;
    void render_menu() override;
    void init() override {};
};
