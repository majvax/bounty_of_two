#include "../include/imgui/imgui.h"
#include "../include/imgui/rlImGui.h"


namespace ImGui
{
    class Context
    {
    private:
		bool m_begin = false;
    public:
        explicit Context(bool darkTheme)
        {
			::rlImGuiSetup(darkTheme);
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
    };
}
