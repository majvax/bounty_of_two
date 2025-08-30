#pragma once

#include "base.hpp"

constexpr float START_ANGLE = 0.0F;
constexpr float DEFAULT_ANGLE_SPEED_X = 0.8F;
constexpr float DEFAULT_ANGLE_SPEED_Y = 1.1F;
constexpr float DEFAULT_DISTANCE = 4.0F;
constexpr float DEFAULT_SCALE = 260.F;


class SceneCube : public SceneABC
{
private:
    float angleX = START_ANGLE;
    float angleY = START_ANGLE;
    float angleSpeedX = DEFAULT_ANGLE_SPEED_X;
    float angleSpeedY = DEFAULT_ANGLE_SPEED_Y;
    float distance = DEFAULT_DISTANCE;
    float scale = DEFAULT_SCALE;
    sf::VertexArray mesh{ sf::PrimitiveType::Triangles };


public:
    explicit SceneCube(Engine& engine, Client* client) : SceneABC(engine, client) {};
    SceneCube(const SceneCube&) = delete;
    SceneCube& operator=(const SceneCube&) = delete;
    SceneCube(SceneCube&&) = delete;
    SceneCube& operator=(SceneCube&&) = delete;

    ~SceneCube() override = default;

    void update(float deltaTime) override;
    void render(sf::RenderTarget& target) override;
    void handleEvent(const sf::Event& event) override;
    void render_menu() override;
    void init() override {};
};
