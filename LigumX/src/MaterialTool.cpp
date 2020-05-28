#include "MaterialTool.h"
#include "GUI.h"
#include "Entity.h"
#include "Editor.h"
#include "PickingTool.h"
#include "Model.h"
#include "Material.h"

#pragma region  CLASS_SOURCE MaterialTool

#include "MaterialTool.h"
#include "serializer.h"
#include "Entity.h"
#include "Material.h"
const ClassPropertyData MaterialTool::g_Properties[] = 
{
{ "PickedEntity", PIDX_PickedEntity, offsetof(MaterialTool, m_PickedEntity), 0, LXType_ObjectPtr, sizeof(Entity*), LXType_Entity, true, LXType_None, false, PropertyFlags_Transient, 0, 0, 0,}, 
{ "PickedMaterials", PIDX_PickedMaterials, offsetof(MaterialTool, m_PickedMaterials), 0, LXType_stdvector, sizeof(std::vector<Material*>), LXType_stdvector, false, LXType_Material, true, PropertyFlags_Transient, 0, 0, 0,}, 
};
void MaterialTool::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
}
bool MaterialTool::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool MaterialTool::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_PickedEntity], &m_PickedEntity  );
	ImguiHelpers::ShowProperty3(this, g_Properties[PIDX_PickedMaterials], m_PickedMaterials  );
	return true;
}
void MaterialTool::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	MaterialTool* other = (MaterialTool*) otherObj;
	other->SetPickedEntity(m_PickedEntity);
	other->SetPickedMaterials(m_PickedMaterials);
}
const char* MaterialTool::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE MaterialTool

bool MaterialTool::Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance)
{
	return false;
}

void MaterialTool::DrawImguiWindow()
{
	g_GUI->BeginWindow(1000, 700, 0, 0, "Material Tool");

	// todo jpp hack : custom "show property grid" oh well
	m_PickedEntity = g_Editor->GetPickingTool()->GetPickedEntity();
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_PickedEntity], &m_PickedEntity);

	if (m_PickedEntity)
	{
		Model* model = m_PickedEntity->GetModel();

		if (model)
		{
			std::vector<Material*>& materials = model->GetMaterials();
			ImguiHelpers::ShowProperty3(this, g_Properties[PIDX_PickedMaterials], materials);
		}
	}


	g_GUI->EndWindow();

}
