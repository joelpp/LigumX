#include "ObjectTool.h"
#include "GUI.h"

#pragma region  CLASS_SOURCE ObjectTool

#include "ObjectTool.h"
#include "serializer.h"
void ObjectTool::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
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


void ObjectTool::DrawImguiWindow()
{
	if (!GetVisible())
	{
		return;
	}

	g_GUI->BeginWindow(1000, 700, 0, 0, "Object Tool");


	g_GUI->EndWindow();
}
