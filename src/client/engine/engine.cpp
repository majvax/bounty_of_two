#include "engine.hpp"
#include "imgui.h"
#include "scene/base.hpp"

Engine::Engine()
  : window(sf::VideoMode::getDesktopMode(), "bounty_of_two", sf::Style::Default, sf::State::Fullscreen, ctxSetting)
{
    constexpr auto target_framerate = 0; // 0 means no limit

    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(target_framerate);
    if (!ImGui::SFML::Init(window)) { spdlog::get("engine")->error("Failed to initialize ImGui-SFML"); }
    ImGui::GetIO().IniFilename = nullptr;
};

Engine::~Engine() { ImGui::SFML::Shutdown(); }

void Engine::pushScene(std::unique_ptr<SceneABC> scene)
{
    defer([this, scene = std::move(scene)]() mutable {
        const auto logger = spdlog::get("engine");
        if (!scene) {
            logger->error("Attempted to push a null scene");
            return;
        }
        logger->info("Pushing scene: {}", typeid(scene).name());
        scenes_.emplace_back(std::move(scene));
        scenes_.back()->init();
    });
}

void Engine::popScene()
{
    defer([this]() {
        const auto logger = spdlog::get("engine");
        if (scenes_.empty()) {
            logger->warn("Attempted to pop a scene from an empty stack");
            return;
        }

        const auto& scene = *scenes_.back();
        logger->info("Popping scene: {}", typeid(scene).name());
        scenes_.pop_back();
    });
}

void Engine::clearScenes()
{
    defer([this]() {
        const auto logger = spdlog::get("engine");
        if (scenes_.empty()) {
            logger->warn("Attempted to clear an empty scene stack");
            return;
        }

        logger->info("Clearing all scenes");
        scenes_.clear();
    });
}


void Engine::update(float deltaTime)
{
    for (auto& scene : scenes_) { scene->update(deltaTime); }
}

void Engine::render(sf::RenderTarget& target)
{
    for (const auto& scene : scenes_) { scene->render(target); }
}

void Engine::render_menu()
{
    if (scenes_.empty()) {
        spdlog::get("engine")->warn("No scenes to render menu for");
        return;
    }

    scenes_.back()->render_menu();
}

void Engine::handleEvent(const sf::Event& event)
{
    if (scenes_.empty()) {
        spdlog::get("engine")->warn("No scenes to handle event for");
        return;
    }
    scenes_.back()->handleEvent(event);
}


void Engine::run()
{
    const auto logger = spdlog::get("engine");
    sf::Clock clock;

    while (window.isOpen()) {
        process_deferred_task();


        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>()) {
                logger->info("Closing window");
                window.close();
            }
            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->scancode == sf::Keyboard::Scan::Escape) {
                    logger->info("Escape key pressed, closing window");
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


void Engine::process_deferred_task()
{
    while (!deferred_tasks.empty()) {
        auto task = std::move(deferred_tasks.front());
        deferred_tasks.pop();
        task();
    }
}
