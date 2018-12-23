#include "GUI.h"
#include "LigumX.h"
#include "MainWindow.h"

#pragma region  CLASS_SOURCE GUI
GUI* g_GUI;

#include "GUI.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData GUI::g_Properties[] = 
{
{ "m_Enabled", PIDX_m_Enabled, offsetof(GUI, m_m_Enabled), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "NumActiveWindows", PIDX_NumActiveWindows, offsetof(GUI, m_NumActiveWindows), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX, 0,}, 
};
bool GUI::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
void GUI::ShowPropertyGrid()
{
	LXIMGUI_SHOW_BOOL("m_Enabled", m_m_Enabled);
	LXIMGUI_SHOW_INT("NumActiveWindows", m_NumActiveWindows, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX);
}

#pragma endregion  CLASS_SOURCE GUI


GUI::GUI()
{
	MainWindow* window = LigumX::GetRenderer()->m_Window;
	m_WindowHWPtr = window->GetHWObject();

	ImGui_ImplGlfwGL3_Init(m_WindowHWPtr, true);

}

void GUI::BeginWindow(unsigned int xSize, unsigned int ySize, ImGuiWindowFlags flags, bool open, const char* name)
{
	ImGui::SetNextWindowSize(ImVec2((float)xSize, (float)ySize), ImGuiSetCond_FirstUseEver);

	if (!ImGui::Begin(name, &open, flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
}


void GUI::EndWindow()
{
	ImGui::End();
}

void GUI::BeginFrame()
{
	m_NumActiveWindows = 0;
}

void GUI::EndFrame()
{

}

bool GUI::ProcessMouseScroll(float xOffset, float yOffset)
{
	ImGui_ImplGlfwGL3_ScrollCallback(m_WindowHWPtr, xOffset, yOffset);

	return ImGui::IsMouseHoveringAnyWindow();
}
