#include "scene.hpp"

class SceneContinuousSpawn : public Scene {
    Renderer renderer;
    GameState game_state;
    int max_enemies_on_screen;
    int window_width;
    int window_height;
public:
    SceneContinuousSpawn(int width, int height, int max_enemies_on_screen);
    void update(float deltatime, ImGui::Context& ctx);
    void draw(ImGui::Context& ctx);

    Renderer& GetRenderer() { return renderer; }
    GameState& GetGameState() { return game_state; }
};

