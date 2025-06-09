---@diagnostic disable: undefined-global
require "tools/export-compile-commands"


workspace "bounty_of_two"
    configurations { "debug", "release" }
    architecture "x86_64"
    location "build"


project "imgui"
    kind "StaticLib"
    language "C++"
    cppdialect "c++23"
    targetdir "bin/%{cfg.buildcfg}"

    files {
        "include/imgui/imgui.cpp",
        "include/imgui/imgui_draw.cpp",
        "include/imgui/imgui_tables.cpp",
        "include/imgui/imgui_widgets.cpp",
        "include/imgui/imgui_demo.cpp",
    }
    includedirs { "include/imgui" }

    filter "configurations:debug"
        symbols "On"

    filter "configurations:release"
        optimize "Speed"


project "bounty_of_two"
    kind "ConsoleApp"
    language "C++"
    cppdialect "c++23"
    targetdir "bin/%{cfg.buildcfg}"

    -- need to include the headers files
    -- or visual studio won't show them at all
    files { 
        "src/**.cpp",
        "src/**.hpp",
        "include/**.hpp",
        "include/**.h",
        "include/imgui/rlImGui.cpp" 
    }
    includedirs { "include/imgui", "include/raylib", "include" }


    links { "imgui" }

    filter { "system:windows", "action:vs*" }
        includedirs { "include/raylib/msvc" }
        libdirs { "libs/msvc" }
        links { "raylib.lib", "winmm.lib", "Ws2_32.lib" }
        defines { "_WINDOWS" }

    filter { "system:windows", "toolset:gcc" }
        includedirs { "include/raylib/mingw" }
        libdirs { "libs/mingw" }
        links { "raylib", "winmm", "gdi32", "ws2_32"  }
        defines { "_WINDOWS" }

    filter { "system:linux" }
        includedirs { "include/raylib/linux" }
        libdirs { "libs/linux" }
        links { "raylib", "pthread", "dl", "rt" }
        defines { "_LINUX" }

    filter "configurations:debug"
        symbols "On"
        defines { "DEBUG" }

    filter "configurations:release"
        optimize "Speed"
