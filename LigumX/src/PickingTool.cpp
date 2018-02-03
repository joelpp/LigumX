#include "PickingTool.h"

#include "LigumX.h"
#include "World.h"

#include "InputHandler.h"

#include "MathUtils.h"

#include "Renderer.h"
#include "Camera.h"

#pragma region  CLASS_SOURCE PickingTool

#include "PickingTool.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData PickingTool::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(PickingTool, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", PIDX_Name, offsetof(PickingTool, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "Enabled", PIDX_Enabled, offsetof(PickingTool, m_Enabled), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "PickedEntity", PIDX_PickedEntity, offsetof(PickingTool, m_PickedEntity), 0, LXType_Entity, true, LXType_None, PropertyFlags_Hidden | PropertyFlags_Transient, 0, 0, }, 
{ "PickedWorldPosition", PIDX_PickedWorldPosition, offsetof(PickingTool, m_PickedWorldPosition), 0, LXType_glmvec3, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "PickingData", PIDX_PickingData, offsetof(PickingTool, m_PickingData), 0, LXType_glmvec4, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "PickedDepth", PIDX_PickedDepth, offsetof(PickingTool, m_PickedDepth), 0, LXType_float, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "PickedID", PIDX_PickedID, offsetof(PickingTool, m_PickedID), 0, LXType_float, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "PickDebugModels", PIDX_PickDebugModels, offsetof(PickingTool, m_PickDebugModels), 0, LXType_float, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
};
bool PickingTool::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE PickingTool


bool PickingTool::Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance)
{
	if (!m_Enabled || !mouseButton1Down)
	{
		return false;
	}

	World* world = LigumX::GetInstance().GetWorld();

	for (Entity* entity : world->GetEntities())
	{
		// todo : proper int rendertarget; how does depth work then? do we care?
		if (MathUtils::FuzzyEquals(m_PickedID, entity->GetPickingID(), 0.005f))
		{
			m_PickedEntity = entity;

			g_DefaultObjects->DefaultManipulatorEntity->SetPosition(m_PickedEntity->GetPosition());

			// todo : ressuscitate this!
			//Renderer* renderer = LigumX::GetRenderer();
			//renderer->RenderEntityBB(m_PickedEntity);

			break;
		}
	}

	return true;
}

void PickingTool::UpdatePickingData()
{
	Renderer* renderer = LigumX::GetRenderer();
	World* world = LigumX::GetInstance().GetWorld();

	renderer->RenderPickingBuffer(m_PickDebugModels);

	const glm::vec2& mousePosition = g_InputHandler->GetMousePosition();

	renderer->GetPickingData(mousePosition, m_PickingData);

	SetPickedWorldPosition(glm::vec3(m_PickingData));
	SetPickedID(m_PickingData[3]);

	const glm::vec3& cameraPosition = renderer->GetDebugCamera()->GetPosition();

	float depth = glm::length(m_PickedWorldPosition - cameraPosition);
	SetPickedDepth(depth);
}

