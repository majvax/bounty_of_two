#include "test.hpp"
#include "imgui.h"
#include "scene/cube.hpp"
#include "scene/title.hpp"
#include "visitor.hpp"
#include <spdlog/spdlog.h>


constexpr auto get_color_visitor = make_visitor([](const player_t&) { return sf::Color::Green; },
  [](const berserker_t&) { return sf::Color::Black; },
  [](const sniper_t&) { return sf::Color::Blue; });


constexpr auto render_visitor = make_visitor([](const auto& entity, sf::RenderTarget& target) {
    sf::Color col = get_color_visitor(entity);
    if (entity.has_flag(EntityFlags::Damaged)) { col = sf::Color::Red; }
    sf::FloatRect bounds = entity.get_bounds();
    sf::RectangleShape shape(bounds.size);
    shape.setPosition(bounds.position);
    shape.setFillColor(col);
    target.draw(shape);
});


void TestScene::render(sf::RenderTarget& target)
{
    for (const auto& entity : state.entities) { visit_ctx(render_visitor, entity, target); }
}

void TestScene::update(float deltaTime)
{
    const auto result = client->receive([this, deltaTime](net::message_type type, auto& packet) {
        switch (type) {
        case net::message_type::GameUpdate: {
            net::gamestate_packet_t state_packet;
            packet >> state_packet;
            state = state_packet.to_gamestate();
            break;
        }
        default:
            spdlog::warn("Unknown message type received in TestScene: {}", static_cast<uint8_t>(type));
            break;
        }
    });

    if (result && *result == receive_status::WouldBlock) {
        // No data received, just update the game state
        // this will smoothly animate entities even when no updates are received
        state.update(deltaTime);
    }
}

void TestScene::handleEvent(const sf::Event& event)
{
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->scancode == sf::Keyboard::Scan::Enter) {
            spdlog::info("Enter key pressed, clearing scene and adding next scene");
            engine.clearScenes();
            engine.pushScene(std::make_unique<TitleScene>(engine, client));
            engine.pushScene(std::make_unique<SceneCube>(engine, client));
        }
    }
}

void TestScene::render_menu()
{
    constexpr ImVec2 window_size = { 100, 100 };
    ImGui::SetNextWindowSize(window_size, ImGuiCond_Once);
    ImGui::Begin("Main Window");

    std::string fps_text = "FPS: " + std::to_string(ImGui::GetIO().Framerate);
    ImGui::TextUnformatted(fps_text.c_str());

    ImGui::End();
}
