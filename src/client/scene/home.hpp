#pragma once
#include "base.hpp"
#include <array>
#include <cstddef>
#include <functional>
#include <optional>

constexpr auto font_size = 42;
constexpr auto outline_thickness = 2.0F;

struct MenuItem
{
    std::optional<sf::Text> text;
    std::move_only_function<void()> action;

    MenuItem() = default;
    MenuItem(sf::Font& font, const std::string& label, std::move_only_function<void()> foo) : action(std::move(foo))
    {
        text.emplace(font, label, font_size);
        text->setFillColor(sf::Color::Black);
        text->setOutlineColor(sf::Color::Black);
        text->setOutlineThickness(outline_thickness);
    }
};

template<std::uint8_t Size>
struct Menu
{
    std::array<MenuItem, Size> items;
    std::uint8_t selected_index = 0;
    sf::RectangleShape selection_rect;
    bool is_dirty = true;

    Menu()
    {
        constexpr auto color = sf::Color(93, 104, 138);
        selection_rect.setFillColor(color);
        selection_rect.setOutlineColor(sf::Color::Black);
        selection_rect.setOutlineThickness(outline_thickness);
    }

    void move_up()
    {
        if (selected_index == 0) {
            selected_index = items.size() - 1;
        } else {
            --selected_index;
        }
    }
    void move_down() { selected_index = (selected_index + 1) % items.size(); }
    void select()
    {
        if (items[selected_index].action) { items[selected_index].action(); }
    }
};

class HomeScene : public SceneABC
{
private:
    std::optional<sf::Text> title_text;
    Menu<3> menu;
    bool join_menu_active = false;
    std::array<std::string, 4> server_ip{ "127", "0", "0", "1" };

    void update_layout();
    void host_callback();
    void join_callback();
    auto mouse_hover(const sf::Vector2f& mouse_pos) const -> std::optional<std::uint8_t>;
public:
    HomeScene(Engine& eng, Client* cli);

    void update(float deltaTime) override;
    void render(sf::RenderTarget& target) override;
    void handleEvent(const sf::Event& event) override;
    void render_menu() override;
    void init() override{};
};
