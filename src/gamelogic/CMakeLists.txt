add_library(gamelogic STATIC
    state.cpp
)

target_include_directories(gamelogic
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(gamelogic
    common
)
