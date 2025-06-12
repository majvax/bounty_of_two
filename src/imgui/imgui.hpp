#pragma once
#include <imgui.h>
#include <raylib-cpp.hpp>
#include <rlImGui.h>
#include <vector>
#include <memory>

#include "../utils/observer_ptr.hpp"
#include "../entity/player.hpp"


namespace ImGui
{
    class Context
    {
    private:
		bool m_begin = false;

        observer_ptr<Player> m_player{nullptr};
        observer_ptr<raylib::Window> m_window{nullptr};

    public:
        explicit Context(bool darkTheme)
        {
			::rlImGuiSetup(darkTheme);
            ImGui::GetIO().IniFilename = nullptr;
        }

        explicit Context(bool darkTheme, Player* player, raylib::Window* window)
            : m_player(player), m_window(window)
        {
            ::rlImGuiSetup(darkTheme);
            ImGui::GetIO().IniFilename = nullptr;
        }


		~Context()
		{
			::rlImGuiShutdown();
		}

        bool lock()
        {
            if (m_begin) {
                ::rlImGuiEnd();
                m_begin = false;
            }
            else {
                ::rlImGuiBegin();
                m_begin = true;
            }

            return m_begin;
        }
    
        bool is_locked() const
        {
            return m_begin;
        }

        void setPlayer(Player* player) { m_player = player; }
        void setWindow(raylib::Window* window) { m_window = window; }

        auto getPlayer() const { return m_player; }
        auto getWindow() const { return m_window; }
    };
}
