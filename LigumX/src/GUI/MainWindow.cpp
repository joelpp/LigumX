#include "MainWindow.h"
#include "Logging.h"
#include "EngineSettings.h"

#pragma region  CLASS_SOURCE MainWindow

#include "MainWindow.h"
#include "serializer.h"
const ClassPropertyData MainWindow::g_Properties[] = 
{
{ "InFocus", PIDX_InFocus, offsetof(MainWindow, m_InFocus), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, PropertyFlags_Transient, 0, 0, 0,}, 
{ "Position", PIDX_Position, offsetof(MainWindow, m_Position), 0, LXType_glmvec2, sizeof(glm::vec2), LXType_glmvec2, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "Size", PIDX_Size, offsetof(MainWindow, m_Size), 0, LXType_glmivec2, sizeof(glm::ivec2), LXType_glmivec2, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "Title", PIDX_Title, offsetof(MainWindow, m_Title), 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, false, 0, 0, 0, 0,}, 
};
void MainWindow::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeVec2(g_Properties[PIDX_Position], m_Position);
	serializer.SerializeIVec2(g_Properties[PIDX_Size], m_Size);
	serializer.SerializeString(g_Properties[PIDX_Title], m_Title);
}
bool MainWindow::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	PostSerialization(writing, success);
	return success;
}
bool MainWindow::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_InFocus], &m_InFocus  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Position], &m_Position , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Size], &m_Size , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowString2(this, g_Properties[PIDX_Title], m_Title  );
	return true;
}
void MainWindow::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	MainWindow* other = (MainWindow*) otherObj;
	other->SetInFocus(m_InFocus);
	other->SetPosition(m_Position);
	other->SetSize(m_Size);
	other->SetTitle(m_Title);
}
const char* MainWindow::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE MainWindow

MainWindow::MainWindow()
{

}

void MainWindow::PostSerialization(bool writing, bool success)
{
}


void MainWindow::Initialize()
{
	pWindow = GL::CreateGLWindow(m_Size.x, m_Size.y, m_Title.c_str());

	glfwSetWindowPos(pWindow, (int)m_Position.x, (int)m_Position.y);
	glfwMakeContextCurrent(pWindow);
	if (pWindow == NULL)
	{
		PRINT("Failed to open GLFW window.");
		lxAssert0();
		glfwTerminate();
		return;
	}
}
