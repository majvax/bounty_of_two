
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <spdlog/spdlog.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <imgui.h>
#include <imgui-SFML.h>


int main()
{
    constexpr auto anti_aliasing_level = 8;
    constexpr auto target_framerate = 165;
    const auto desktopMode = sf::VideoMode::getDesktopMode();
    spdlog::info("Current desktop video mode: {}x{} @ {} bpp",
                 desktopMode.size.x, desktopMode.size.y, desktopMode.bitsPerPixel);

    sf::ContextSettings ctxSettings;
    ctxSettings.antiAliasingLevel = anti_aliasing_level; 
    sf::RenderWindow window(
        desktopMode, 
      "bounty_of_two",
      sf::Style::Default,
     sf::State::Fullscreen,
      ctxSettings
    );

    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(target_framerate);
    if (!ImGui::SFML::Init(window))
    {
        spdlog::error("Failed to initialize ImGui-SFML");
        return -1;
    }


    sf::Font ui_font;
    constexpr const char* font_file = "C:/Windows/Fonts/segoeui.ttf";

    if (!ui_font.openFromFile(font_file))
    {
        spdlog::error("Failed to load font from: {}", font_file);
        return -1;
    }
    ui_font.setSmooth(true);
    constexpr auto font_size = 42;
    constexpr auto outline_thickness = 2.F;

    sf::Text title(ui_font, "bounty of two", font_size);
    title.setFillColor(sf::Color::Black);
    title.setOutlineColor(sf::Color::White);
    title.setOutlineThickness(outline_thickness);


    constexpr std::array<sf::Vector3f, 8> cube_vertices = {
        sf::Vector3f{-1,-1,-1},
        {1,-1,-1},
        {1,1,-1},
        {-1,1,-1},
        {-1,-1, 1},
        {1,-1, 1},
        {1,1, 1},
        {-1,1, 1}
    };
    constexpr std::array<std::pair<int, int>, 12> cube_edge = {
        std::pair{0,1},
        {1,2},
        {2,3},
        {3,0},
        {4,5},
        {5,6},
        {6,7},
        {7,4},
        {0,4},
        {1,5},
        {2,6},
        {3,7}
    };


    float angleX = 0.F;
    float angleY = 0.F;
    sf::Clock clock;

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
            {
                spdlog::info("Closing window");
                window.close();
            }
            if (const auto *key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->scancode == sf::Keyboard::Scan::Escape)
                {
                    spdlog::info("Escape key pressed, closing window");
                    window.close();
                }
            }
        }

        const auto time = clock.restart();
        ImGui::SFML::Update(window, time);
        constexpr float angleSpeedX = 0.8F;
        constexpr float angleSpeedY = 1.1F;

        float deltaTime = time.asSeconds();
        angleX += deltaTime * angleSpeedX;
        angleY += deltaTime * angleSpeedY;

        constexpr float distance = 4.0F;
        constexpr float scale = 260.F;
        constexpr float centerScale = 0.5F;
        sf::Vector2f center{static_cast<float>(window.getSize().x) * centerScale, static_cast<float>(window.getSize().y) * centerScale};


        constexpr float minZ = 0.2F;
        auto project = [&](const auto &vec) -> sf::Vector2f {
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
            return {center.x + (x2 * scale * persp), center.y + (y2 * scale * persp)};
        };

        // Build thick line wireframe using triangles
        constexpr float line_thickness = 4.F;
        constexpr float half = 0.5F;
        sf::VertexArray mesh(sf::PrimitiveType::Triangles);
        constexpr float min_segment_length = 0.0001F;

        auto appendThickSegment = [&](sf::Vector2f vec_a, sf::Vector2f vec_b, sf::Color col)
        {
            sf::Vector2f vec_d = vec_b - vec_a;
            float len = std::sqrt((vec_d.x * vec_d.x) + (vec_d.y * vec_d.y));
            if (len < min_segment_length) {
                return;
            }

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

        for (auto [pt_a, pt_b] : cube_edge)
        {
            auto vec_a = project(cube_vertices.at(pt_a));
            auto vec_b = project(cube_vertices.at(pt_b));
            appendThickSegment(vec_a, vec_b, sf::Color::Black);
        }

        constexpr auto background_color = sf::Color(245, 245, 250);
        window.clear(background_color);
        window.draw(mesh);
            
        
        const auto bounds = title.getLocalBounds();
        const auto window_size = static_cast<sf::Vector2f>(window.getSize());
        float x = ((window_size.x - bounds.size.x) * half) - bounds.position.x;
        constexpr float position_y = 0.25F;
        float y = ((window_size.y - bounds.size.y) * position_y) - bounds.position.y;
            
        title.setPosition({x, y});
        window.draw(title);
        ImGui::ShowDemoWindow();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}
