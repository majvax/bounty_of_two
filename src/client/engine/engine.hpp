#pragma once
#include <vector>
#include <memory>
#include "scene/base.hpp"
#include <spdlog/spdlog.h>
#include <ImGui-SFML.h>

constexpr sf::ContextSettings ctxSetting{
    .depthBits = 24,
    .stencilBits = 8,
    .antiAliasingLevel = 8
};



class Engine {
    std::vector<std::unique_ptr<SceneABC>> scenes_;
    sf::RenderWindow window;

public:
    Engine(): window(sf::VideoMode::getDesktopMode(), "bounty_of_two", sf::Style::Default, sf::State::Fullscreen, ctxSetting) {
        constexpr auto anti_aliasing_level = 8;
        constexpr auto target_framerate = 165;

        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(target_framerate);
        if (!ImGui::SFML::Init(window)) {
            spdlog::error("Failed to initialize ImGui-SFML");
        }
    };
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(Engine&&) = delete;
    ~Engine() {
        ImGui::SFML::Shutdown();
    }

    template<typename Scenetype>
    void pushScene() {
        scenes_.emplace_back(std::make_unique<Scenetype>(window));
        scenes_.back()->init();
    }

    void popScene() {
        if (!scenes_.empty()) {
            scenes_.pop_back();
        }
    }

    void clearScenes() {
        scenes_.clear();
    }

    void update(float deltaTime) {
        for (auto &scenePtr : scenes_) {
            scenePtr->update(deltaTime);
        }
    }

    void render(sf::RenderTarget& target) {
        for (auto &scenePtr : scenes_) {
            scenePtr->render(target);
        }
    }

    void render_menu() {
        if (!scenes_.empty()) {
            scenes_.back()->render_menu();
        }
    }

    void handleEvent(const sf::Event& event) {
        scenes_.back()->handleEvent(event);
    }

    void run() {
        sf::Clock clock;

        while (window.isOpen()) {
            while (const auto event = window.pollEvent()) {
                ImGui::SFML::ProcessEvent(window, *event);

                if (event->is<sf::Event::Closed>()) {
                    spdlog::info("Closing window");
                    window.close();
                }
                if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                    if (key->scancode == sf::Keyboard::Scan::Escape) {
                        spdlog::info("Escape key pressed, closing window");
                        window.close();
                    }
                }
                handleEvent(*event);
            }

            const auto time = clock.restart();
            ImGui::SFML::Update(window, time);

            
            float deltaTime = time.asSeconds();

            constexpr auto background_color = sf::Color(245, 245, 250);
            window.clear(background_color);

            update(deltaTime);
            render(window);
            render_menu();



            ImGui::SFML::Render(window);
            window.display();
        }
    }



};
