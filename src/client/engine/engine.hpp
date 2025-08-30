#pragma once
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/ContextSettings.hpp"
#include <SFML/Window/Event.hpp>
#include <vector>
#include <memory>
#include <spdlog/spdlog.h>
#include <imgui-SFML.h>
#include <queue>
#include <functional>


class SceneABC;


constexpr sf::ContextSettings ctxSetting{
    .depthBits = 24,
    .stencilBits = 8,
    .antiAliasingLevel = 16
};



class Engine {
    std::vector<std::unique_ptr<SceneABC>> scenes_;
    sf::RenderWindow window;
    std::queue<std::move_only_function<void()>> deferred_tasks;

    bool in_render{ false };
    bool in_update{ false };

public:
    Engine();
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(Engine&&) = delete;
    ~Engine();


    void pushScene(std::unique_ptr<SceneABC> scene);
    void popScene();
    void clearScenes();
    void update(float deltaTime);
    void render(sf::RenderTarget& target);
    void render_menu();
    void handleEvent(const sf::Event& event);
    void run();
    void process_deferred_task();
    void add_deferred_task(std::move_only_function<void()> task) {
        deferred_tasks.emplace(std::move(task));
    }

    [[nodiscard]] const std::vector<std::unique_ptr<SceneABC>>& getScenes() const { return scenes_; }
    [[nodiscard]] std::vector<std::unique_ptr<SceneABC>>& getScenes() { return scenes_; }

    [[nodiscard]] const sf::RenderWindow& getWindow() const { return window; }
    [[nodiscard]] sf::RenderWindow& getWindow() { return window; }

};
