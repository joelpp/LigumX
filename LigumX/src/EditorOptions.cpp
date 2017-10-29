#include "EditorOptions.h"

#pragma region  CLASS_SOURCE EditorOptions

#include "EditorOptions.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData EditorOptions::g_Properties[] = 
{
{ "ObjectID", offsetof(EditorOptions, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", offsetof(EditorOptions, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "Enabled", offsetof(EditorOptions, m_Enabled), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowWorldWindow", offsetof(EditorOptions, m_ShowWorldWindow), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowMaterialWindow", offsetof(EditorOptions, m_ShowMaterialWindow), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowEntityWindow", offsetof(EditorOptions, m_ShowEntityWindow), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowMaterialCreator", offsetof(EditorOptions, m_ShowMaterialCreator), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowEngineStats", offsetof(EditorOptions, m_ShowEngineStats), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowTestGUI", offsetof(EditorOptions, m_ShowTestGUI), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowObjectManager", offsetof(EditorOptions, m_ShowObjectManager), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "BackupDataOnSave", offsetof(EditorOptions, m_BackupDataOnSave), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
};
void EditorOptions::Serialize(bool writing)
{
	g_Serializer->SerializeObject(this, writing); 
}

#pragma endregion  CLASS_SOURCE EditorOptions

EditorOptions::EditorOptions()
{
	m_ObjectID = g_ObjectManager->GetNewObjectID();
}
