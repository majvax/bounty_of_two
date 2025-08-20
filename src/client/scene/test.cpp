#include "test.hpp"
#include "visitor.hpp"
#include <random>
#include <ranges>
#include <spdlog/spdlog.h>
#include "scene/title.hpp"
#include "scene/cube.hpp"


void TestScene::render(sf::RenderTarget &target) {
  for (const auto &entity : state.entities) {
    const auto visitor = Visitor{[&target](const player_t &ent) {
                                   sf::Color color = sf::Color::Green;
                                   if (ent.has_flags(EntityFlags::Damaged)) {
                                     color = sf::Color::Red;
                                   }
                                   sf::RectangleShape shape(ent.size);
                                   shape.setPosition(ent.position);
                                   shape.setFillColor(color);
                                   target.draw(shape);
                                 },
                                 [&target](const berserker_t &ent) {
                                   sf::Color color = sf::Color::Black;
                                   if (ent.has_flags(EntityFlags::Damaged)) {
                                     color = sf::Color::Red;
                                   }
                                   sf::RectangleShape shape(ent.size);
                                   shape.setPosition(ent.position);
                                   shape.setFillColor(color);
                                   target.draw(shape);
                                 },
                                 [&target](const sniper_t &ent) {
                                   sf::Color color = sf::Color::Blue;
                                   if (ent.has_flags(EntityFlags::Damaged)) {
                                     color = sf::Color::Red;
                                   }
                                   sf::RectangleShape shape(ent.size);
                                   shape.setPosition(ent.position);
                                   shape.setFillColor(color);
                                   target.draw(shape);
                                 }};

    std::visit(visitor, entity);
  }
}

constexpr auto init_visitor = Visitor{[](auto &ent) {
  static std::mt19937 rng(std::random_device{}());
  static std::uniform_real_distribution<float> x_dist(0.F, 1920.F);
  static std::uniform_real_distribution<float> y_dist(0.F, 1080.F);
  static std::uniform_real_distribution<float> vel_dist(-50.F, 50.F);

  ent.position = {x_dist(rng), y_dist(rng)};
  ent.size = {20.F, 20.F};
  ent.health = 100;
  ent.damage = 10;
  ent.velocity = {vel_dist(rng), vel_dist(rng)};
}};

void TestScene::init() {
  // Initialize entities for testing
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

  for (auto &entity : state.entities) {
    std::visit(init_visitor, entity);
  }
}

void TestScene::update(float deltaTime) { state.update(deltaTime); }




void TestScene::handleEvent(const sf::Event &event) {
    if (const auto* key = event.getIf<sf::Event::KeyPressed>())
    {
        if (key->scancode == sf::Keyboard::Scan::Enter) {
            spdlog::info("Enter key pressed, clearing scene and adding next scene");
            engine.clearScenes();
            engine.pushScene(std::make_unique<TitleScene>(engine));
            engine.pushScene(std::make_unique<SceneCube>(engine));
        }
    }
}
