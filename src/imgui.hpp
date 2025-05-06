#include "imgui.h"
#include "rlImGui.h"


namespace ImGui
{
    class Context
    {
    private:
		bool m_begin = false;
    public:
        Context(bool darkTheme)
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