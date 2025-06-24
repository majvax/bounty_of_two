#include "upgrade_scene.hpp"
#include "menu.hpp"
#include "scene.hpp"
#include "../imgui/menu.hpp"
#include <raylib-cpp.hpp>




UpgradeScene::UpgradeScene(int width, int height) : Scene(width, height), 
    upgrades(get_upgrades()) {}

void UpgradeScene::update(float deltatime, ImGui::Context& ctx) {
    using namespace raylib::Keyboard;
    if (raylib::Keyboard::IsKeyPressed(KEY_ONE)) {
        upgrades[0]->execute(GetGameState());
        Scene::PopScene();
    } else if (raylib::Keyboard::IsKeyPressed(KEY_TWO)) {
        upgrades[1]->execute(GetGameState());
        Scene::PopScene();
    } else if (raylib::Keyboard::IsKeyPressed(KEY_THREE)) {
        upgrades[2]->execute(GetGameState());
        Scene::PopScene();
    }
}

void UpgradeScene::draw(ImGui::Context& ctx) {
    raylib::Rectangle overlay(0, 0, GetRenderer().GetWidth(), GetRenderer().GetHeight());
    overlay.Draw({0, 0, 0, 180});

    int boxWidth = 300;
    int boxHeight = 80;
    int spacing = 30;
    int startY = GetRenderer().GetHeight() / 2 - (boxHeight * 3 + spacing * 2) / 2;
    int centerX = GetRenderer().GetWidth() / 2 - boxWidth / 2;
    raylib::Font font;

    for (int i = 0; i < 3; ++i) {
        int y = startY + i * (boxHeight + spacing);
        raylib::Rectangle box(centerX, y, boxWidth, boxHeight);
        box.Draw({40, 40, 40, 220});
        box.DrawLines({255, 255, 255, 255}, 2.0f);
        std::string label = std::to_string(upgrades[i]->get_type());
        raylib::Text indice(font, label, 25);
        indice.Draw({static_cast<float>(centerX) + boxWidth / 2 - indice.Measure() / 2, static_cast<float>(y) + boxHeight / 2 });
    }

    raylib::Text title(font, "Choose an upgrade:", 25);
    title.Draw({static_cast<float>(GetRenderer().GetWidth()) / 2 - title.Measure() / 2, 50});
}
