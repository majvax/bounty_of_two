#include "cube.hpp"
#include <array>
#include <cmath>
#include <imgui.h>
#include <numbers>
#include <spdlog/spdlog.h>
#include "scene/test.hpp"
#include <memory>

constexpr std::array<sf::Vector3f, 8> cube_vertices = { { { -1, -1, -1 },
  { 1, -1, -1 },
  { 1, 1, -1 },
  { -1, 1, -1 },
  { -1, -1, 1 },
  { 1, -1, 1 },
  { 1, 1, 1 },
  { -1, 1, 1 } } };
constexpr std::array<std::pair<int, int>, 12> cube_edge = { { { 0, 1 },
  { 1, 2 },
  { 2, 3 },
  { 3, 0 },
  { 4, 5 },
  { 5, 6 },
  { 6, 7 },
  { 7, 4 },
  { 0, 4 },
  { 1, 5 },
  { 2, 6 },
  { 3, 7 } } };


void SceneCube::update(const float deltaTime)
{
    angleX = std::fmod(angleX + (deltaTime * angleSpeedX), std::numbers::pi_v<float> * 2);
    if (angleX < START_ANGLE) { angleX += std::numbers::pi_v<float> * 2; }

    angleY = std::fmod(angleY + (deltaTime * angleSpeedY), std::numbers::pi_v<float> * 2);
    if (angleY < START_ANGLE) { angleY += std::numbers::pi_v<float> * 2; }

    constexpr float centerScale = 0.5F;
    sf::Vector2f center{ static_cast<float>(engine.getWindow().getSize().x) * centerScale,
        static_cast<float>(engine.getWindow().getSize().y) * centerScale };


    constexpr float minZ = 0.2F;
    auto project = [&](const auto& vec) -> sf::Vector2f {
        float cx = std::cos(angleX);
        float sx = std::sin(angleX);
        float cy = std::cos(angleY);
        float sy = std::sin(angleY);

        // rotate X
        float y1 = (vec.y * cx) - (vec.z * sx);
        float z1 = (vec.y * sx) + (vec.z * cx);

        float x1 = vec.x;
        // rotate Y
        float x2 = (x1 * cy) + (z1 * sy);
        float z2 = (-x1 * sy) + (z1 * cy);
        float y2 = y1;
        float zc = z2 + distance;
        zc = std::max(zc, minZ);

        // perspective projection
        float persp = 1.F / zc;
        return { center.x + (x2 * scale * persp), center.y + (y2 * scale * persp) };
    };


    // Build thick line wireframe using triangles
    constexpr float line_thickness = 4.F;
    constexpr float half = 0.5F;
    constexpr float min_segment_length = 0.0001F;
    mesh.clear();


    auto appendThickSegment = [&](sf::Vector2f vec_a, sf::Vector2f vec_b, sf::Color col) {
        sf::Vector2f vec_d = vec_b - vec_a;
        float len = std::sqrt((vec_d.x * vec_d.x) + (vec_d.y * vec_d.y));
        if (len < min_segment_length) { return; }

        sf::Vector2f normal = { -vec_d.y / len, vec_d.x / len };
        sf::Vector2f off = normal * (line_thickness * half);

        sf::Vector2f corner0 = vec_a + off;
        sf::Vector2f corner1 = vec_a - off;
        sf::Vector2f corner2 = vec_b - off;
        sf::Vector2f corner3 = vec_b + off;

        mesh.append(sf::Vertex(corner0, col));
        mesh.append(sf::Vertex(corner1, col));
        mesh.append(sf::Vertex(corner2, col));
        mesh.append(sf::Vertex(corner0, col));
        mesh.append(sf::Vertex(corner2, col));
        mesh.append(sf::Vertex(corner3, col));
    };


    for (auto [pt_a, pt_b] : cube_edge) {
        auto vec_a = project(cube_vertices.at(pt_a));
        auto vec_b = project(cube_vertices.at(pt_b));
        appendThickSegment(vec_a, vec_b, sf::Color::Black);
    }
}


void SceneCube::render(sf::RenderTarget& target) { target.draw(mesh); }


constexpr float MIN_VALUE = 0.F;
constexpr float MAX_VALUE = 10.F;
constexpr float SCALE_MAX_VALUE = 1000.F;


void SceneCube::render_menu()
{
    constexpr ImVec2 window_size = { 400, 300 };
    ImGui::SetNextWindowSize(window_size, ImGuiCond_Once);
    ImGui::Begin("Main Window");
    ImGui::SliderFloat("speed x", &angleSpeedX, MIN_VALUE, MAX_VALUE);
    ImGui::SliderFloat("speed y", &angleSpeedY, MIN_VALUE, MAX_VALUE);
    ImGui::SliderFloat("distance", &distance, MIN_VALUE, MAX_VALUE);
    ImGui::SliderFloat("scale", &scale, MIN_VALUE, SCALE_MAX_VALUE);
    ImGui::SliderFloat("Current angle X", &angleX, MIN_VALUE, std::numbers::pi_v<float> * 2);// 2*PI
    ImGui::SliderFloat("Current angle Y", &angleY, MIN_VALUE, std::numbers::pi_v<float> * 2);// 2*PI

    std::string fps_text = "FPS: " + std::to_string(ImGui::GetIO().Framerate);
    ImGui::TextUnformatted(fps_text.c_str());

    ImGui::End();
}



void SceneCube::handleEvent(const sf::Event& event)
{
    if (const auto* key = event.getIf<sf::Event::KeyPressed>())
    {
        if (key->scancode == sf::Keyboard::Scan::Enter) {
            spdlog::info("Enter key pressed, clearing scene and adding next scene");
            engine.clearScenes();
            engine.pushScene(std::make_unique<TestScene>(engine));
        }
    }
}
