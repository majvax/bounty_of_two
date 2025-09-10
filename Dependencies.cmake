include(cmake/CPM.cmake)


cpmaddpackage("gh:facebook/zstd@1.5.7")

cpmaddpackage("gh:fmtlib/fmt#11.2.0")

cpmaddpackage(URI "gh:gabime/spdlog@1.15.3" OPTIONS "SPDLOG_FMT_EXTERNAL ON")

# cpmaddpackage(
#     NAME freetype
#     GIT_TAG VER-2-14-0
#     GITHUB_REPOSITORY freetype/freetype
# )

cpmaddpackage(
    NAME sfml
    GIT_TAG 3.0.1
    GITHUB_REPOSITORY SFML/SFML
    OPTIONS
    "SFML_BUILD_EXAMPLES OFF"
    "SFML_BUILD_TESTING OFF"
)

cpmaddpackage(URI "gh:ocornut/imgui@1.92.0" DOWNLOAD_ONLY)
add_library(imgui STATIC
  "${imgui_SOURCE_DIR}/imgui.cpp"
  "${imgui_SOURCE_DIR}/imgui_draw.cpp"
  "${imgui_SOURCE_DIR}/imgui_tables.cpp"
  "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
  "${imgui_SOURCE_DIR}/imgui_demo.cpp"
  "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp"
  "${imgui_SOURCE_DIR}/misc/freetype/imgui_freetype.cpp"
)
target_include_directories(imgui SYSTEM PUBLIC "${imgui_SOURCE_DIR}" "${FREETYPE_INCLUDE_DIR_freetype2}")
target_compile_definitions(imgui PRIVATE IMGUI_ENABLE_FREETYPE)
target_link_libraries(imgui PRIVATE SFML::Graphics)

cpmaddpackage(
    NAME imgui-sfml
    GITHUB_REPOSITORY majvax/imgui-sfml
    GIT_TAG b1900f4ffe25c995a54551baf809139e4b09c7e3
    OPTIONS
    "IMGUI_SFML_FIND_SFML OFF"
    "IMGUI_DIR ${imgui_SOURCE_DIR}"
)

target_include_directories(ImGui-SFML PRIVATE "${imgui_SOURCE_DIR}")


cpmaddpackage(
    NAME half
    GITHUB_REPOSITORY suruoxi/half
    GIT_TAG 7cd91f2a3b5feba92a0eb44ed314e0ddb9962d89
    DOWNLOAD_ONLY YES
)
add_library(half INTERFACE)
target_include_directories(half SYSTEM INTERFACE "${half_SOURCE_DIR}/include")
