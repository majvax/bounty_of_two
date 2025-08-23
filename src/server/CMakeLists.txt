add_executable(server 
    main.cpp
    )

target_link_libraries(server PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    fmt::fmt
    spdlog::spdlog
    common
    gamelogic
)

target_include_directories(client PRIVATE
    "${imgui_SOURCE_DIR}"
    "./"
)
