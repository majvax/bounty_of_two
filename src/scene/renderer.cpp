#include "renderer.hpp"
#include "game_state.hpp"
#include "../entity/base.hpp"
#include "../imgui/imgui.hpp"
#include "../utils/assert.hpp"

#include "../imgui/fpscounter.hpp"
#include "../imgui/player_gui.hpp"


Renderer::Renderer(int width, int height): background(raylib::Texture2D("assets/desert.png")), width(width), height(height) {
    background.SetFilter(TEXTURE_FILTER_BILINEAR);

    camera.SetOffset({width / 2.0f, height / 2.0f});
    camera.SetZoom(1.0f);

    background_elements.push_back(raylib::Texture2D("assets/cactus.png"));
    background_elements.push_back(raylib::Texture2D("assets/lac.png"));
    background_elements.push_back(raylib::Texture2D("assets/palmier.png"));
    for (size_t i = 0; i < 50; i++) {
        bgelem_positions.push_back({
            (float)GetRandomValue(0, background.width*4),
            (float)GetRandomValue(0, background.width*4),
            GetRandomValue(0, background_elements.size()-1)
        });
    }

    add_menu(std::make_unique<FPSCounter>());
    add_menu(std::make_unique<PlayerMenu>());
}


void Renderer::add_menu(std::unique_ptr<MenuBase> menu) {
    menus.push_back(std::move(menu));
}

void Renderer::update(ImGui::Context& ctx) const {
    ASSERT_IMGUI_CTX_NOT_LOCKED(ctx);

    for (const auto& menu : menus) {
        menu->update(ctx);
    }
}

void Renderer::draw(ImGui::Context& ctx, const GameState& game_state) {
    // updating the camera position based on the first player which should be the main player

    const auto& players = game_state.GetPlayers();
    if (!players.empty()) {
        ASSERT_PTR(players.front(), "Player must not be null");
        auto position = players.front()->GetCenter();
        camera.target = { position.x, position.y };
        ctx.setPlayer(players.front().get());
    }
    

    // drawing all the entities
    camera.BeginMode();
    
    float camLeft = camera.target.x - camera.offset.x / camera.zoom;
    float camTop = camera.target.y - camera.offset.y / camera.zoom;
    // float camRight = camLeft + GetScreenWidth() / camera.zoom;
    // float camBottom = camTop + GetScreenHeight() / camera.zoom;

    int texW = background.width;
    int texH = background.height;

    // Calculate start positions with modulo for smooth repeating
    int startX = static_cast<int>(camLeft) - (static_cast<int>(camLeft) % texW) - texW;
    int startY = static_cast<int>(camTop) - (static_cast<int>(camTop) % texH) - texH;

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

    int startelemX = static_cast<int>(camLeft) - (static_cast<int>(camLeft) % (texW*4)) - (texW*4);
    int startelemY = static_cast<int>(camTop) - (static_cast<int>(camTop) % (texH*4)) - (texH*4);

    for (int y = 0; y < tilesY; y++) {
        for (int x = 0; x < tilesX; x++) {
            for (raylib::Vector3 bgelempos : bgelem_positions) {
                background_elements[(int)bgelempos.z].Draw(
                    startelemX + bgelempos.x + x * texW*4,
                    startelemY + bgelempos.y + x * texW*4,
                    WHITE
                );
            }
        }
    }

    for (const auto& player : game_state.GetPlayers()) {
        player->draw();
    }

	for (const auto& entity : game_state.GetEntities()) {
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
