#include "PickingTool.h"

#include "LigumX.h"
#include "World.h"
#include "Sector.h"

#include "InputHandler.h"

#include "MathUtils.h"
#include "RaycastingHelpers.h"

#include "Renderer.h"
#include "RenderDataManager.h"
#include "Camera.h"
#include "BoundingBoxComponent.h"

#pragma region  CLASS_SOURCE PickingTool

#include "PickingTool.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData PickingTool::g_Properties[] = 
{
{ "Enabled", PIDX_Enabled, offsetof(PickingTool, m_Enabled), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "PickedEntity", PIDX_PickedEntity, offsetof(PickingTool, m_PickedEntity), 0, LXType_ObjectPtr, sizeof(Entity*), LXType_Entity, true, LXType_None, false, PropertyFlags_SetCallback | PropertyFlags_Transient, 0, 0, WriteSetFunction(PickingTool, PickedEntity, Entity*),}, 
{ "AimingWindowPosition", PIDX_AimingWindowPosition, offsetof(PickingTool, m_AimingWindowPosition), 0, LXType_glmvec2, sizeof(glm::vec2), LXType_glmvec2, false, LXType_None, false, PropertyFlags_Transient, 0, 0, 0,}, 
{ "AimingWorldPosition", PIDX_AimingWorldPosition, offsetof(PickingTool, m_AimingWorldPosition), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, PropertyFlags_Transient, 0, 0, 0,}, 
{ "AimingID", PIDX_AimingID, offsetof(PickingTool, m_AimingID), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, PropertyFlags_Transient, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "PickedWorldPosition", PIDX_PickedWorldPosition, offsetof(PickingTool, m_PickedWorldPosition), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, PropertyFlags_Transient, 0, 0, 0,}, 
{ "PickingData", PIDX_PickingData, offsetof(PickingTool, m_PickingData), 0, LXType_glmvec4, sizeof(glm::vec4), LXType_glmvec4, false, LXType_None, false, PropertyFlags_Hidden | PropertyFlags_Transient, 0, 0, 0,}, 
{ "PickedDepth", PIDX_PickedDepth, offsetof(PickingTool, m_PickedDepth), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, PropertyFlags_Transient, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "PickedID", PIDX_PickedID, offsetof(PickingTool, m_PickedID), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, PropertyFlags_Transient, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "PickDebugModels", PIDX_PickDebugModels, offsetof(PickingTool, m_PickDebugModels), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, PropertyFlags_Transient, 0, 0, 0,}, 
{ "PickedHeight", PIDX_PickedHeight, offsetof(PickingTool, m_PickedHeight), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, PropertyFlags_Transient, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "PickedSector", PIDX_PickedSector, offsetof(PickingTool, m_PickedSector), 0, LXType_ObjectPtr, sizeof(Sector*), LXType_Sector, true, LXType_None, false, PropertyFlags_Transient, 0, 0, 0,}, 
};
bool PickingTool::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
void PickingTool::ShowPropertyGrid()
{
	LXIMGUI_SHOW_BOOL("Enabled", m_Enabled);
	LXIMGUI_SHOW_FLOAT("AimingID", m_AimingID, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX);
	LXIMGUI_SHOW_FLOAT("PickedDepth", m_PickedDepth, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX);
	LXIMGUI_SHOW_FLOAT("PickedID", m_PickedID, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX);
	LXIMGUI_SHOW_BOOL("PickDebugModels", m_PickDebugModels);
	LXIMGUI_SHOW_FLOAT("PickedHeight", m_PickedHeight, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX);
}

#pragma endregion  CLASS_SOURCE PickingTool

bool PickingTool::CheckEntity(Entity* entity)
{
	if (m_AimingID == 0)
	{
		return false;
	}

	Renderer& renderer = Renderer::GetInstance();
	const glm::vec3& cameraPosition = renderer.GetDebugCamera()->GetPosition();

	if (MathUtils::FuzzyEquals(m_AimingID, entity->GetPickingID(), 0.005f))
	{
		SetPickedEntity(entity);

		SetPickedWorldPosition(m_AimingWorldPosition);
		SetPickedID(m_AimingID);

		float depth = glm::length(m_PickedWorldPosition - cameraPosition);

		SetPickedDepth(depth);

		g_DefaultObjects->DefaultManipulatorEntity->SetPosition(m_PickedEntity->GetPosition());
		
		return true;
	}

	return false;
}

void AddEntityMessage(Entity* entity)
{
	std::stringstream msg;
	msg << entity->GetName();
	g_RenderDataManager->AddMouseMessage(msg.str());
}

void PickingTool::SetPickedEntity_Callback(Entity* entity)
{
	// something about debug display / compile out in release
	if (entity)
	{
		AddEntityMessage(entity);

		if (entity != m_PickedEntity)
		{
			BoundingBoxComponent* bb = entity->GetComponent<BoundingBoxComponent>();
			bb->SetUpdatesWithEntity(true);
			entity->UpdateAABB();
		}
	}

	m_PickedEntity = entity;
}

bool PickingTool::Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance)
{
	if (!m_Enabled )
	{
		return false;
	}

	g_RenderDataManager->AddEntityAABB(m_PickedEntity);

	if (mouseButton1Down)
	{
		bool found = false;

		World* world = LigumX::GetInstance().GetWorld();

		for (Entity* entity : g_RenderDataManager->GetVisibleEntities())
		{
			if (CheckEntity(entity))
			{
				found = true;

				SetPickedEntity(entity);

				break;
			};
		}

		if (!found)
		{
			for (Sector* sector : g_RenderDataManager->GetVisibleSectors())
			{
				if (CheckEntity(sector->GetTerrainPatchEntity()))
				{
					found = true;

					SetPickedEntity(sector->GetTerrainPatchEntity());

					break;
				};
			}
		}

		m_PickedSector = g_World->GetSectorByWorldPosition(m_AimingWorldPosition);

		if (!found)
		{
			SetPickedEntity(nullptr);

			g_DefaultObjects->DefaultManipulatorEntity->SetPosition(glm::vec3(0, 0, 0));
		}


	}


	return true;
}

bool PickingTool::IsPickingEntity()
{
	return m_PickedID != 0;
}

void PickingTool::UpdatePickingData()
{
	Renderer* renderer = LigumX::GetRenderer();
	World* world = LigumX::GetInstance().GetWorld();

	renderer->RenderPickingBuffer(m_PickDebugModels);

	const glm::vec2& mousePosition = g_InputHandler->GetMousePosition();

	m_AimingWindowPosition = mousePosition;

	renderer->GetPickingData(mousePosition, m_PickingData);

	SetAimingWorldPosition(glm::vec3(m_PickingData));

	SetAimingID(m_PickingData[3]);

	if (m_AimingID == 0.f)
	{
		// We need to compute aiming world position ourselves
		m_AimingWorldPosition = RaycastingHelpers::GetAimingWorldSpacePosition();
	}


}

