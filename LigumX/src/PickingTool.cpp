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
#include "Entity.h"
#include "Sector.h"
const ClassPropertyData PickingTool::g_Properties[] = 
{
{ "Enabled", PIDX_Enabled, offsetof(PickingTool, m_Enabled), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "PickedEntity", PIDX_PickedEntity, offsetof(PickingTool, m_PickedEntity), 0, LXType_ObjectPtr, sizeof(Entity*), LXType_Entity, true, LXType_None, false, PropertyFlags_SetCallback | PropertyFlags_Transient, 0, 0, WriteSetFunction(PickingTool, PickedEntity, Entity*),}, 
{ "AimingWindowPosition", PIDX_AimingWindowPosition, offsetof(PickingTool, m_AimingWindowPosition), 0, LXType_glmvec2, sizeof(glm::vec2), LXType_glmvec2, false, LXType_None, false, PropertyFlags_Transient, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "AimingWorldPosition", PIDX_AimingWorldPosition, offsetof(PickingTool, m_AimingWorldPosition), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, PropertyFlags_Transient, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "AimingID", PIDX_AimingID, offsetof(PickingTool, m_AimingID), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, PropertyFlags_Transient, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "PickedWorldPosition", PIDX_PickedWorldPosition, offsetof(PickingTool, m_PickedWorldPosition), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, PropertyFlags_Transient, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "PickingData", PIDX_PickingData, offsetof(PickingTool, m_PickingData), 0, LXType_glmvec4, sizeof(glm::vec4), LXType_glmvec4, false, LXType_None, false, PropertyFlags_Hidden | PropertyFlags_Transient, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "PickedDepth", PIDX_PickedDepth, offsetof(PickingTool, m_PickedDepth), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, PropertyFlags_Transient, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "PickedID", PIDX_PickedID, offsetof(PickingTool, m_PickedID), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, PropertyFlags_Transient, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "PickDebugModels", PIDX_PickDebugModels, offsetof(PickingTool, m_PickDebugModels), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, PropertyFlags_Transient, 0, 0, 0,}, 
{ "PickedHeight", PIDX_PickedHeight, offsetof(PickingTool, m_PickedHeight), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, PropertyFlags_Transient, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "PickedSector", PIDX_PickedSector, offsetof(PickingTool, m_PickedSector), 0, LXType_ObjectPtr, sizeof(Sector*), LXType_Sector, true, LXType_None, false, PropertyFlags_Transient, 0, 0, 0,}, 
{ "CloneEntity", PIDX_CloneEntity, offsetof(PickingTool, m_CloneEntity), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, PropertyFlags_SetCallback | PropertyFlags_Transient, 0, 0, WriteSetFunction(PickingTool, CloneEntity, bool),}, 
};
void PickingTool::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool(g_Properties[PIDX_Enabled], m_Enabled);
}
bool PickingTool::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool PickingTool::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Enabled], &m_Enabled  );
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_PickedEntity], &m_PickedEntity  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_AimingWindowPosition], &m_AimingWindowPosition , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_AimingWorldPosition], &m_AimingWorldPosition , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_AimingID], &m_AimingID , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_PickedWorldPosition], &m_PickedWorldPosition , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_PickingData], &m_PickingData , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_PickedDepth], &m_PickedDepth , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_PickedID], &m_PickedID , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_PickDebugModels], &m_PickDebugModels  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_PickedHeight], &m_PickedHeight , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_PickedSector], &m_PickedSector  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_CloneEntity], &m_CloneEntity  );
	return true;
}
const char* PickingTool::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE PickingTool

bool PickingTool::CheckEntity(Entity* entity)
{
	if (m_AimingID == 0)
	{
		return false;
	}

	Renderer& renderer = Renderer::GetInstance();
	const glm::vec3& cameraPosition = renderer.GetActiveCamera()->GetPosition();

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

			if (bb)
			{
				bb->SetUpdatesWithEntity(true);
				entity->UpdateAABB();
			}
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

void PickingTool::SetCloneEntity_Callback(const bool& value)
{
	if (value)
	{
		if (m_PickedEntity != nullptr)
		{
			Entity* entity = new Entity();
			*entity = *m_PickedEntity;
			entity->SetObjectID(rand());
			entity->GetComponents().clear();

			LXString name = entity->GetName();
			name += " " + std::to_string(g_World->GetEntities().size());
			entity->SetName(name);

			glm::vec3 offset = glm::linearRand(-1.f * glm::vec3(15.f, 15.f, 15.f), glm::vec3(15.f, 15.f, 15.f));
			entity->AddTo_Position(offset);
			entity->SetHasMoved(true);

			g_World->AddTo_Entities(entity);
		}

		m_CloneEntity = false;
	}
}
