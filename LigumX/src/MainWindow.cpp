#include "MainWindow.h"
#include "Logging.h"
#include "EngineSettings.h"

#pragma region  CLASS_SOURCE MainWindow

#include "MainWindow.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData MainWindow::g_Properties[] = 
{
{ "InFocus", PIDX_InFocus, offsetof(MainWindow, m_InFocus), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Position", PIDX_Position, offsetof(MainWindow, m_Position), 0, LXType_glmvec2, sizeof(glm::vec2), LXType_glmvec2, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "Size", PIDX_Size, offsetof(MainWindow, m_Size), 0, LXType_glmivec2, sizeof(glm::ivec2), LXType_glmivec2, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Title", PIDX_Title, offsetof(MainWindow, m_Title), 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, false, 0, 0, 0, 0,}, 
};
bool MainWindow::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	PostSerialization(writing, success);
	return success;
}
bool MainWindow::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	LXIMGUI_SHOW_BOOL("InFocus", m_InFocus);
	LXIMGUI_SHOW_VEC2("Position", m_Position, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX);
	LXIMGUI_SHOW_STRING("Title", m_Title);
	return true;
}

#pragma endregion  CLASS_SOURCE MainWindow

MainWindow::MainWindow()
{

}

MainWindow::MainWindow(int objectID)
{
	SetObjectID(objectID);
	Serialize(false);
}

void MainWindow::PostSerialization(bool writing, bool success)
{
	if (!writing)
	{
		pWindow = GL::CreateGLWindow(m_Size.x, m_Size.y, m_Title.c_str());

		glfwSetWindowPos(pWindow, m_Position.x, m_Position.y);
		glfwMakeContextCurrent(pWindow);
		if (pWindow == NULL)
		{
			PRINTSTRING("Failed to open GLFW window.");
			glfwTerminate();
			return;
		}
	}
}
