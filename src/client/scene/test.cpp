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
    const auto result = client->receive([this](net::message_type type, auto& packet) {
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
    using net::input_type;


    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        // TODO: refactor input handling to make automatic cast to underlying type
        // and more convenient bitwise operations.
        // FIXME: Currently only one input can be sent at a time due to the switch-case structure.
        // This should be changed to allow multiple inputs to be sent in a single packet.
        // e.g. pressing W and A at the same time should move the player diagonally
        // For this I need to create a build_input_mask() function that is called every frame
        // with a throttle to avoid spamming the server with packets. Using sfml isKeyPressed
        // function to check the state of each key every frame and build the input mask accordingly.
        uint8_t input = 0;
        switch (key->scancode) {
        case sf::Keyboard::Scan::W: input |= static_cast<uint8_t>(input_type::MoveUp) ; break;
        case sf::Keyboard::Scan::A: input |= static_cast<uint8_t>(input_type::MoveLeft); break;
        case sf::Keyboard::Scan::S: input |= static_cast<uint8_t>(input_type::MoveDown); break;
        case sf::Keyboard::Scan::D: input |= static_cast<uint8_t>(input_type::MoveRight); break;
        default:
            break;
        }

        if (input != static_cast<uint8_t>(input_type::None)) {
            net::packet_t<net::message_type::PlayerInput> packet;
            packet << net::input_packet_t{ input };
            client->send_data(packet);
        }



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
