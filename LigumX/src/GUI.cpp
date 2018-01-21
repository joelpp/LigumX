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
{ "ObjectID", PIDX_ObjectID, offsetof(GUI, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", PIDX_Name, offsetof(GUI, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "m_Enabled", PIDX_m_Enabled, offsetof(GUI, m_m_Enabled), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "NumActiveWindows", PIDX_NumActiveWindows, offsetof(GUI, m_NumActiveWindows), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
};
bool GUI::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE GUI


GUI::GUI()
{
	MainWindow* window = LigumX::GetRenderer()->m_Window;
	ImGui_ImplGlfwGL3_Init(window->GetHWObject(), true);
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