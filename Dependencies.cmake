include(cmake/CPM.cmake)


cpmaddpackage("gh:facebook/zstd@1.5.7")

cpmaddpackage("gh:fmtlib/fmt#11.2.0")

cpmaddpackage(URI "gh:gabime/spdlog@1.15.3" OPTIONS "SPDLOG_FMT_EXTERNAL ON")

cpmaddpackage(
    NAME sfml
    GIT_TAG 3.0.1
    GITHUB_REPOSITORY SFML/SFML
    OPTIONS 
    "SFML_BUILD_EXAMPLES OFF"
    "SFML_BUILD_TESTING OFF"
)

cpmaddpackage(URI "gh:ocornut/imgui@1.91.1" DOWNLOAD_ONLY)
add_library(imgui STATIC  
  "${imgui_SOURCE_DIR}/imgui.cpp"  
  "${imgui_SOURCE_DIR}/imgui_draw.cpp"  
  "${imgui_SOURCE_DIR}/imgui_tables.cpp"  
  "${imgui_SOURCE_DIR}/imgui_widgets.cpp"  
  "${imgui_SOURCE_DIR}/imgui_demo.cpp"  
)
target_include_directories(imgui SYSTEM PUBLIC "${imgui_SOURCE_DIR}")

cpmaddpackage(
    NAME imgui-sfml
    VERSION 3.0
    GITHUB_REPOSITORY SFML/imgui-sfml
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
