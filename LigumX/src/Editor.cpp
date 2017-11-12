#include "Editor.h"

#pragma region  CLASS_SOURCE Editor

#include "Editor.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData Editor::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(Editor, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", PIDX_Name, offsetof(Editor, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "Options", PIDX_Options, offsetof(Editor, m_Options), 0, LXType_EditorOptions, false, LXType_None, 0, 0, 0, }, 
};
bool Editor::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE Editor
