#pragma once

#include "base.hpp"


class TitleScene: public SceneABC
{
private:
    sf::Text titleText;

public:
    explicit TitleScene(Engine& engine);

    void update(float deltaTime) override;
    void render(sf::RenderTarget& target) override { target.draw(titleText); }
    void handleEvent(const sf::Event& event) override {}
    void render_menu() override {}
    void init() override {}

};
