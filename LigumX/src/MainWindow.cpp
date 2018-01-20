#include "MainWindow.h"

#pragma region  CLASS_SOURCE MainWindow

#include "MainWindow.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData MainWindow::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(MainWindow, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", PIDX_Name, offsetof(MainWindow, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "InFocus", PIDX_InFocus, offsetof(MainWindow, m_InFocus), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "Position", PIDX_Position, offsetof(MainWindow, m_Position), 0, LXType_glmvec2, false, LXType_None, 0, 0, 0, }, 
{ "Size", PIDX_Size, offsetof(MainWindow, m_Size), 0, LXType_glmvec2, false, LXType_None, 0, 0, 0, }, 
};
bool MainWindow::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE MainWindow