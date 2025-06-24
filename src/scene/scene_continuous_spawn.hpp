#include "scene.hpp"

class SceneContinuousSpawn : public Scene {
    int max_enemies_on_screen;
    int window_width;
    int window_height;
    int score;
public:
    SceneContinuousSpawn(int width, int height, int max_enemies_on_screen);
    void update(float deltatime, ImGui::Context& ctx) override;
    void draw(ImGui::Context& ctx) override;
};

