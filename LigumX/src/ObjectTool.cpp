#include "ObjectTool.h"
#include "GUI.h"

#pragma region  CLASS_SOURCE ObjectTool

#include "ObjectTool.h"
#include "serializer.h"
const ClassPropertyData ObjectTool::g_Properties[] = 
{
{ "SelectedFileIndex", PIDX_SelectedFileIndex, offsetof(ObjectTool, m_SelectedFileIndex), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "LoadUnloadedObjects", PIDX_LoadUnloadedObjects, offsetof(ObjectTool, m_LoadUnloadedObjects), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "COMMAND_SaveCurrentObject", PIDX_COMMAND_SaveCurrentObject, offsetof(ObjectTool, m_COMMAND_SaveCurrentObject), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, PropertyFlags_SetCallback, 0, 0, WriteSetFunction(ObjectTool, COMMAND_SaveCurrentObject, bool),}, 
{ "COMMAND_CreateNewObject", PIDX_COMMAND_CreateNewObject, offsetof(ObjectTool, m_COMMAND_CreateNewObject), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, PropertyFlags_SetCallback, 0, 0, WriteSetFunction(ObjectTool, COMMAND_CreateNewObject, bool),}, 
};
void ObjectTool::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeInt(g_Properties[PIDX_SelectedFileIndex], m_SelectedFileIndex);
	serializer.SerializeBool(g_Properties[PIDX_LoadUnloadedObjects], m_LoadUnloadedObjects);
	serializer.SerializeBool(g_Properties[PIDX_COMMAND_SaveCurrentObject], m_COMMAND_SaveCurrentObject);
	serializer.SerializeBool(g_Properties[PIDX_COMMAND_CreateNewObject], m_COMMAND_CreateNewObject);
}
bool ObjectTool::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool ObjectTool::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_SelectedFileIndex], &m_SelectedFileIndex , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_LoadUnloadedObjects], &m_LoadUnloadedObjects  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_COMMAND_SaveCurrentObject], &m_COMMAND_SaveCurrentObject  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_COMMAND_CreateNewObject], &m_COMMAND_CreateNewObject  );
	return true;
}
const char* ObjectTool::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE ObjectTool


bool ObjectTool::Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance)
{
	return false;
}

struct LXObjectFilenameData
{
	ObjectID id = 0;
	LXType type;
};

const char* g_ImguiStrings_ObjectTool_CreateNewObject = "Create new object...";
void ObjectTool::DrawImguiWindow()
{

	g_GUI->BeginWindow(1000, 700, 0, 0, "Object Tool");

	ShowPropertyGrid();

	std::vector<FileDisplayInformation>& allFiles = g_ObjectManager->GetAllFiles();

	int selectedFileIndex = m_SelectedFileIndex;

	int numItems = allFiles.size();
	ImGui::ListBox("Symbols", &selectedFileIndex, ImguiHelpers::VectorOfFileDisplayInfoGetter, (void*)&allFiles, numItems);

	if (selectedFileIndex != -1)
	{
		bool selectionChanged = (selectedFileIndex != m_SelectedFileIndex);
		if (selectionChanged)
		{
			m_SelectedFileIndex = selectedFileIndex;

			m_CurrentObject = g_ObjectManager->GetObjectFromIDAndType(m_LoadUnloadedObjects, allFiles[m_SelectedFileIndex].m_ObjectID, allFiles[m_SelectedFileIndex].m_Typename);
		}

		ImGui::Text("%s", allFiles[m_SelectedFileIndex].m_AsText.c_str());
	}

	bool showPropertyGrid = (m_CurrentObject != nullptr);
	if (showPropertyGrid)
	{
		m_CurrentObject->ShowPropertyGrid();
	}

	{
		const char* names[] = { "Entity", "SunLight", "Material" };

		// Simple selection popup
		// (If you want to show the current selection inside the Button itself, you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
		if (ImGui::BeginPopup(g_ImguiStrings_ObjectTool_CreateNewObject))
		{
			for (int i = 0; i < 3 /*IM_ARRAYSIZE(names)*/; i++)
			{
				if (ImGui::Selectable(names[i]))
				{
					CreateNewObject(names[i]);
				}
			}
			ImGui::EndPopup();
		}
	}

	g_GUI->EndWindow();
}

void ObjectTool::SetCOMMAND_SaveCurrentObject_Callback(const bool& value)
{
	m_CurrentObject->Serialize(true);
	g_ObjectManager->UpdateFileList();
}

void ObjectTool::CreateNewObject(const LXString& typeName)
{
	LXObject* newObject = g_ObjectManager->CreateNewObject(typeName);
	m_CurrentObject = newObject;
	m_SelectedFileIndex = -1;
}

void ObjectTool::SetCOMMAND_CreateNewObject_Callback(const bool& value)
{
	m_COMMAND_CreateNewObject = value;
	ImGui::OpenPopup(g_ImguiStrings_ObjectTool_CreateNewObject);
}
