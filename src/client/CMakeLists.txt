add_executable(client
    main.cpp
    engine/engine.cpp
    scene/cube.cpp
    scene/title.cpp
    scene/game.cpp
    scene/home.cpp
)

target_link_libraries(client PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
    fmt::fmt
    spdlog::spdlog
    ImGui-SFML::ImGui-SFML
    imgui
    common
    gamelogic
    resources
)

target_include_directories(client PRIVATE
    "${imgui_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_set_warning(client ${WARNINGS_AS_ERRORS})
