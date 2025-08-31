add_executable(server
    main.cpp
    networking/server.cpp
    networking/callback.cpp
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

target_set_warning(server ${WARNINGS_AS_ERRORS})
