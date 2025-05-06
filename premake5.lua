workspace "bounty_of_two"
    configurations { "Debug", "Release" }
    architecture "x86_64"
    location "build"

project "bounty_of_two"
    kind "ConsoleApp"
    language "C++"
    cppdialect "c++23"
    targetdir "bin/%{cfg.buildcfg}"

    files { "src/**.cpp", "src/**.hpp", "include/**.hpp", "include/**.h", "include/**.cpp" }
    includedirs { "include/imgui", "include/raylib" }

    filter { "system:windows", "action:vs*" }
        includedirs { "include/raylib/msvc" }
        libdirs { "libs/msvc" }
        links { "raylib.lib", "winmm.lib" }
        defines { "_WINDOWS" }

    filter { "system:windows", "toolset:gcc" }
        includedirs { "include/raylib/mingw" }
        libdirs { "libs/mingw" }
        links { "raylib", "winmm", "gdi32",  }
        defines { "_WINDOWS" }

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"
