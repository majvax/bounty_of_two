add_library(common INTERFACE)
target_include_directories(common INTERFACE  
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>  
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>  
)

# Propagate SFML headers required for entities.hpp (Vector2)
target_link_libraries(common INTERFACE SFML::System SFML::Graphics)
