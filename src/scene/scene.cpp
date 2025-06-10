#include "scene.hpp"
#include <ranges>
#include <algorithm>
#include "../imgui/imgui.hpp"
#include "../imgui/menu.hpp"
#include "../utils/assert.hpp"

Scene::Scene(int width, int height) {
    background = raylib::Texture2D("assets/background.jpg");
    background.SetFilter(TEXTURE_FILTER_BILINEAR);

    camera.SetOffset({width / 2.0f, height / 2.0f});
    camera.SetZoom(1.0f);
}

void Scene::update(float deltaTime) {
    // Updating each entity with the delta time
	for (auto& entity : entities) {
        entity->update(deltaTime);
    }


    // Updating the camera position based on the player position
    if (auto player = get_player()) {
        ASSERT_PTR(player, "Player must not be null");
        auto position = player->GetCenter();
        camera.target = { position.x, position.y };
    }
}

void Scene::draw(ImGui::Context& ctx) {
    // drawing all the entities
    camera.BeginMode();
    
    float camLeft = camera.target.x - camera.offset.x / camera.zoom;
    float camTop = camera.target.y - camera.offset.y / camera.zoom;
    // float camRight = camLeft + GetScreenWidth() / camera.zoom;
    // float camBottom = camTop + GetScreenHeight() / camera.zoom;

    int texW = background.width;
    int texH = background.height;

    // Calculate start positions with modulo for smooth repeating
    int startX = (int)(camLeft) - ((int)(camLeft) % texW) - texW;
    int startY = (int)(camTop) - ((int)(camTop) % texH) - texH;

    // Calculate how many tiles needed to cover the screen plus margin
    int tilesX = (GetScreenWidth() / texW) + 3;
    int tilesY = (GetScreenHeight() / texH) + 3;

    for (int y = 0; y < tilesY; y++) {
        for (int x = 0; x < tilesX; x++) {
            background.Draw(
                startX + x * texW,
                startY + y * texH,
                WHITE
            );
        }
    }


	for (const auto& entity : entities) {
        entity->draw();
    }

    camera.EndMode();

    // drawing all the menus
    while (ctx.lock()) {
        for (const auto& menu : menus) {
            menu->render(ctx);
        }
    }
}


void Scene::add_entity(std::unique_ptr<EntityBase> entity) {
    // Moving the ownership of the entity into the vector
    ASSERT_PTR(entity, "Entity must not be null");
    entities.push_back(std::move(entity));
}

void Scene::remove_entity(EntityBase* entity) {
    ASSERT_PTR(entity, "Entity must not be null");
    auto it = std::ranges::find_if(entities, [entity](const auto& ptr) {
        return ptr.get() == entity;
    });
    if (it != entities.end()) {
        entities.erase(it);
    }
}


void Scene::add_menu(std::unique_ptr<MenuBase> menu) {
    ASSERT_PTR(menu, "Menu must not be null");
    menus.push_back(std::move(menu));
}

void Scene::remove_menu(MenuBase* menu) {
    ASSERT_PTR(menu, "Menu must not be null");
    auto it = std::ranges::find_if(menus, [menu](const auto& ptr) {
        return ptr.get() == menu;
    });
    if (it != menus.end()) {
        menus.erase(it);
    }
}


Player* Scene::get_player() const {
    // Assuming the first entity is always the player
    if (entities.empty()) return nullptr;
    return static_cast<Player*>(entities.front().get());
}
