#include "MainWindow.h"
#include "Logging.h"
#include "EngineSettings.h"

#pragma region  CLASS_SOURCE MainWindow

#include "MainWindow.h"
#include "serializer.h"
const ClassPropertyData MainWindow::g_Properties[] = 
{
{ "InFocus", PIDX_InFocus, offsetof(MainWindow, m_InFocus), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Position", PIDX_Position, offsetof(MainWindow, m_Position), 0, LXType_glmvec2, sizeof(glm::vec2), LXType_glmvec2, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "Size", PIDX_Size, offsetof(MainWindow, m_Size), 0, LXType_glmivec2, sizeof(glm::ivec2), LXType_glmivec2, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "Title", PIDX_Title, offsetof(MainWindow, m_Title), 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, false, 0, 0, 0, 0,}, 
};
void MainWindow::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool(g_Properties[PIDX_InFocus], m_InFocus);
	serializer.SerializeVec2(g_Properties[PIDX_Position], m_Position);
	serializer.SerializeIVec2(g_Properties[PIDX_Size], m_Size);
	serializer.SerializeString(g_Properties[PIDX_Title], m_Title);
	serializer.Close();
}
bool MainWindow::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	PostSerialization(writing, success);
	return success;
}
bool MainWindow::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowBool("InFocus", m_InFocus  );
	ImguiHelpers::ShowVec2("Position", m_Position , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowIVec2("Size", m_Size , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowString("Title", m_Title  );
	return true;
}
const char* MainWindow::GetTypeName()
{
	return ClassName;
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

		glfwSetWindowPos(pWindow, (int)m_Position.x, (int)m_Position.y);
		glfwMakeContextCurrent(pWindow);
		if (pWindow == NULL)
		{
			PRINTSTRING("Failed to open GLFW window.");
			glfwTerminate();
			return;
		}
	}
}
