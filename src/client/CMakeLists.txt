add_executable(client 
    main.cpp
    engine/engine.cpp
    scene/cube.cpp
    scene/title.cpp

    )

target_link_libraries(client PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    fmt::fmt
    spdlog::spdlog
    ImGui-SFML::ImGui-SFML
    imgui
)

target_include_directories(client PRIVATE
    "${imgui_SOURCE_DIR}"
    "./"
)
