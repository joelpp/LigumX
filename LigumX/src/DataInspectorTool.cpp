#include "DataInspectorTool.h"
#include "GUI.h"
#include "renderer.h"

#pragma region  CLASS_SOURCE DataInspectorTool

#include "DataInspectorTool.h"
#include "serializer.h"
void DataInspectorTool::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
}
bool DataInspectorTool::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool DataInspectorTool::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	return true;
}
const char* DataInspectorTool::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE DataInspectorTool

void DataInspectorTool::DrawImguiWindow()
{
	g_GUI->BeginWindow(1000, 700, 0, 0, "Data Inspector Tool");

	ShowPropertyGrid();

	Renderer& renderer = Renderer::GetInstance();
	float* values = renderer.GetDataInspectorValues();

	for (int i = 0; i < DATAINSPECTOR_NUMVALUES; ++i)
	{
		std::string text = lxFormat("[%d] = %f", i, values[i]);
		ImguiHelpers::ShowRawString(text);
	}

	g_GUI->EndWindow();

}