#include "home.hpp"
#include "SFML/Network/IpAddress.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Mouse.hpp"
#include "config.hpp"
#include "scene/game.hpp"
#include "scene/home.hpp"
#include <algorithm>
#include <cstdint>
#include <helper.hpp>
#include <imgui.h>
#include <limits>
#include <memory>
#include <misc/cpp/imgui_stdlib.h>
#include <optional>
#include <ranges>

HomeScene::HomeScene(Engine& eng, Client* cli) : SceneABC(eng, cli)
{
    auto font = Config::default_font().value();
    title_text.emplace(font, "bounty of two", font_size);

    menu.items = { { { font, "host game", [this]() { this->host_callback(); } },
      { font, "join game", [this]() { this->join_callback(); } },
      { font, "quit", [this]() { this->engine.close(); } } } };

    title_text->setFillColor(sf::Color::Black);
    title_text->setOutlineColor(sf::Color::White);
    title_text->setOutlineThickness(outline_thickness);
}

void HomeScene::update_layout()
{
    auto [width, height] = static_cast<sf::Vector2f>(engine.getWindow().getSize());

    // Title: centered, 10% from top
    if (title_text) {
        title_text->setCharacterSize(static_cast<unsigned int>(height * 0.08f));
        auto bounds = title_text->getLocalBounds().size;
        title_text->setOrigin({ bounds.x / 2.f, bounds.y / 2.f });
        title_text->setPosition({ width / 2.f, height * 0.12f });
    }

    // Menu items: centered, spaced
    float menu_start_y = height * 0.35f;
    float menu_spacing = height * 0.15f;

    for (const auto [index, item] : menu.items | std::views::enumerate) {
        if (item.text) {
            item.text->setCharacterSize(static_cast<unsigned int>(height * 0.06f));
            auto bounds = item.text->getLocalBounds().size;
            item.text->setOrigin({ bounds.x / 2.f, bounds.y / 2.f });
            item.text->setPosition({ width / 2.f, menu_start_y + (static_cast<float>(index) * menu_spacing) });
        }
    }

    // Selection rectangle: match the selected menu item
    const auto& selected_item = menu.items.at(menu.selected_index);
    if (selected_item.text) {
        auto bounds = selected_item.text->getGlobalBounds().size;
        menu.selection_rect.setSize({ bounds.x + 100.F, bounds.y + 40.0F });
        menu.selection_rect.setOrigin({ menu.selection_rect.getSize().x / 2.f, (menu.selection_rect.getSize().y - 25.0F) / 2.f });
        menu.selection_rect.setPosition(selected_item.text->getPosition());
    }

    menu.is_dirty = false;
}


void HomeScene::update(float /*deltaTime*/)
{
    if (menu.is_dirty) { update_layout(); }
}

void HomeScene::render(sf::RenderTarget& target)
{
    if (title_text) { target.draw(*title_text); }

    target.draw(menu.selection_rect);

    for (const auto [index, item] : menu.items | std::views::enumerate) {
        if (!item.text) { continue; }
        auto& text = *item.text;
        if (index == menu.selected_index) {
            constexpr auto color = sf::Color::White;
            text.setFillColor(color);
            text.setOutlineColor(color);
        } else {
            text.setFillColor(sf::Color::Black);
            text.setOutlineColor(sf::Color::Black);
        }
        target.draw(*item.text);
    }
}


void HomeScene::host_callback()
{
    if (!client->connect()) {
        spdlog::get("app")->critical("Server isn't running at localhost. Please run client first");
    }
    engine.clearScenes();
    engine.pushScene(std::make_unique<GameScene>(engine, client));
}

void HomeScene::join_callback()
{
    if (!join_menu_active) {
        join_menu_active = true;
        return;
    }

    const auto max = std::numeric_limits<std::uint8_t>::max();
    const auto min = std::numeric_limits<std::uint8_t>::min();

    sf::IpAddress s_ip(std::clamp(to_u8(server_ip.at(0)), min, max),
      std::clamp(to_u8(server_ip.at(1)), min, max),
      std::clamp(to_u8(server_ip.at(2)), min, max),
      std::clamp(to_u8(server_ip.at(3)), min, max));

    if (!client->connect(s_ip)) {
        spdlog::get("app")->critical(
          "Couldn't connect to server at {}. Please check the address and try again.", s_ip.toString());
        return;
    }
    engine.clearScenes();
    engine.pushScene(std::make_unique<GameScene>(engine, client));
}

void HomeScene::handleEvent(const sf::Event& event)
{
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->scancode == sf::Keyboard::Scan::Up) {
            menu.move_up();
            menu.is_dirty = true;
        } else if (key->scancode == sf::Keyboard::Scan::Down) {
            menu.move_down();
            menu.is_dirty = true;
        } else if (key->scancode == sf::Keyboard::Scan::Enter) {
            menu.select();
        }
    }

    if (const auto* mouse_moved = event.getIf<sf::Event::MouseMoved>()) {
        const sf::Vector2f mouse_pos{ static_cast<float>(mouse_moved->position.x),
            static_cast<float>(mouse_moved->position.y) };

        if (auto hovered = mouse_hover(mouse_pos)) {
            if (menu.selected_index != *hovered) {
                menu.selected_index = *hovered;
                menu.is_dirty = true;
            }
        }
        return;
    }

    if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mbp->button == sf::Mouse::Button::Left) {
            const sf::Vector2f mouse_pos{ static_cast<float>(mbp->position.x), static_cast<float>(mbp->position.y) };

            if (auto hovered = mouse_hover(mouse_pos)) {
                if (menu.selected_index != *hovered) {
                    menu.selected_index = *hovered;
                    menu.is_dirty = true;
                }
                menu.select();
            }
        }
        return;
    }
}

void HomeScene::render_menu()
{
    if (!join_menu_active) { return; }

    const auto wsize = engine.getWindow().getSize();
    const auto size = static_cast<sf::Vector2f>(wsize) * 0.8F;
    const auto pos = (static_cast<sf::Vector2f>(wsize) - size) / 2.0F;

    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    constexpr auto flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize // NOLINT
                           | ImGuiWindowFlags_NoTitleBar;                                                    // NOLINT
    ImGui::Begin("Menu", nullptr, flags);

    ImGui::PushFont(nullptr, font_size);

    ImGui::TextUnformatted("server ip address : ");
    const auto width = ImGui::GetFont()->CalcTextSizeA(font_size, FLT_MAX, 0.0F, "888").x;

    for (auto [index, elem] : server_ip | std::views::enumerate) {
        if (index != 0) {
            ImGui::SameLine();
            ImGui::TextUnformatted(".");
        }
        ImGui::SameLine();
        ImGui::PushID(static_cast<int>(index));
        constexpr auto input_flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank // NOLINT
                                     | ImGuiInputTextFlags_CharsDecimal;                                  // NOLINT
        ImGui::SetNextItemWidth(width);
        ImGui::InputText("##ip", &elem, input_flags);
        if (elem.size() > 3) { elem.resize(3); }
        ImGui::PopID();
    }

    if (ImGui::Button("connect", { -1.0F, 50.0F })) { join_callback(); }
    if (ImGui::Button("cancel", { -1.0F, 50.0F })) { join_menu_active = false; }


    ImGui::PopFont();
    ImGui::End();
}


std::optional<std::size_t> HomeScene::mouse_hover(const sf::Vector2f& mouse_pos) const
{
    for (auto [index, item] : menu.items | std::views::enumerate) {
        if (item.text) {
            // Global bounds includes the text’s transform
            const auto bounds = item.text->getGlobalBounds();
            if (bounds.contains(mouse_pos)) { return static_cast<std::size_t>(index); }
        }
    }
    return std::nullopt;
}
