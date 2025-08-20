#include "test.hpp"
#include "imgui.h"
#include "scene/cube.hpp"
#include "scene/title.hpp"
#include "visitor.hpp"
#include <random>
#include <ranges>
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

constexpr auto init_visitor = make_visitor([](auto& ent) {
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_real_distribution<float> x_dist(0.F, 1920.F);
    static std::uniform_real_distribution<float> y_dist(0.F, 1080.F);
    static std::uniform_real_distribution<float> vel_dist(-50.F, 50.F);

    ent.position = { x_dist(rng), y_dist(rng) };
    ent.size = { 20.F, 20.F };
    ent.health = 100;
    ent.damage = 10;
    ent.velocity = { vel_dist(rng), vel_dist(rng) };
});

void TestScene::init()
{
    // FIXME: Initialize entities for testing, should happen on server side
    // This is just a placeholder to demonstrate the structure
    constexpr int num_entities = 1500;
    state.entities.reserve(num_entities);
    for (auto index : std::views::iota(0, num_entities)) {
        if (index % 3 == 0) {
            state.entities.emplace_back(berserker_t{});
        } else if (index % 3 == 1) {
            state.entities.emplace_back(sniper_t{});
        } else {
            state.entities.emplace_back(player_t{});
        }
    }

    for (auto& entity : state.entities) { std::visit(init_visitor, entity); }
}

void TestScene::update(float deltaTime) { state.update(deltaTime); }


void TestScene::handleEvent(const sf::Event& event)
{
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->scancode == sf::Keyboard::Scan::Enter) {
            spdlog::info("Enter key pressed, clearing scene and adding next scene");
            engine.clearScenes();
            engine.pushScene(std::make_unique<TitleScene>(engine));
            engine.pushScene(std::make_unique<SceneCube>(engine));
        }
    }
}


void TestScene::render_menu()
{
    ImGui::SetNextWindowSize({ 100, 100 }, ImGuiCond_Once);
    ImGui::Begin("Main Window");

    std::string fps_text = "FPS: " + std::to_string(ImGui::GetIO().Framerate);
    ImGui::TextUnformatted(fps_text.c_str());

    ImGui::End();
}
