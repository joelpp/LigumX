#include "ObjectTool.h"
#include "GUI.h"

#pragma region  CLASS_SOURCE ObjectTool

#include "ObjectTool.h"
#include "serializer.h"
const ClassPropertyData ObjectTool::g_Properties[] = 
{
{ "SelectedFileIndex", PIDX_SelectedFileIndex, offsetof(ObjectTool, m_SelectedFileIndex), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
};
void ObjectTool::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeInt(g_Properties[PIDX_SelectedFileIndex], m_SelectedFileIndex);
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

bool VectorOfStringGetter(void* data, int n, const char** out_text)
{
	std::vector<std::string>* v = (std::vector<std::string>*)data;
	*out_text = (*v)[n].c_str();
	return true;
}

void ObjectTool::DrawImguiWindow()
{

	g_GUI->BeginWindow(1000, 700, 0, 0, "Object Tool");

	std::vector<std::string>& allFiles = g_ObjectManager->GetAllFiles();

	int selectedFileIndex;

	int numItems = allFiles.size();
	ImGui::ListBox("Symbols", &selectedFileIndex, VectorOfStringGetter, (void*)&allFiles, numItems);

	if (selectedFileIndex != m_SelectedFileIndex)
	{
		m_SelectedFileIndex = selectedFileIndex;
	}
	ImGui::Text("%s", allFiles[m_SelectedFileIndex].c_str());


	g_GUI->EndWindow();
}
