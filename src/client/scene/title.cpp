#include "title.hpp"
#include "config.hpp"


constexpr auto font_size = 42;


TitleScene::TitleScene(Engine& eng, Client* cli)
  : SceneABC(eng, cli), titleText(Config::default_font().value(), "bounty of two", font_size)
{
    constexpr float outline_thickness = 2.F;
    titleText.setFillColor(sf::Color::Black);
    titleText.setOutlineColor(sf::Color::White);
    titleText.setOutlineThickness(outline_thickness);
}


void TitleScene::update(float /*deltaTime*/)
{
    constexpr float half = 0.5F;
    const auto bounds = titleText.getLocalBounds();
    const auto window_size = static_cast<sf::Vector2f>(engine.getWindow().getSize());
    float x = ((window_size.x - bounds.size.x) * half) - bounds.position.x;
    constexpr float position_y = 0.25F;
    float y = ((window_size.y - bounds.size.y) * position_y) - bounds.position.y;

    titleText.setPosition({ x, y });
}
