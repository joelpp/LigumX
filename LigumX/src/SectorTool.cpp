#include "SectorTool.h"
#include "LigumX.h"
#include "EngineSettings.h"
#include "InputHandler.h"

#include "Renderer.h"
#include "DebugRenderer.h"
#include "MainWindow.h"
#include "RenderDataManager.h"
#include "GUI.h"

#include "Editor.h"
#include "OSMTool.h"
#include "PickingTool.h"

#include "World.h"
#include "Sector.h"
#include "SectorManager.h"
#include "SectorData.h"
#include "SectorGraphicalData.h"
#include "Way.h"

#pragma region  CLASS_SOURCE SectorTool

#include "SectorTool.h"
#include "serializer.h"
#include "Sector.h"
const ClassPropertyData SectorTool::g_Properties[] = 
{
{ "Enabled", PIDX_Enabled, offsetof(SectorTool, m_Enabled), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "HighlightedWorldCoordinates", PIDX_HighlightedWorldCoordinates, offsetof(SectorTool, m_HighlightedWorldCoordinates), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "HighlightedSectorUV", PIDX_HighlightedSectorUV, offsetof(SectorTool, m_HighlightedSectorUV), 0, LXType_glmvec2, sizeof(glm::vec2), LXType_glmvec2, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "HighlightedSector", PIDX_HighlightedSector, offsetof(SectorTool, m_HighlightedSector), 0, LXType_ObjectPtr, sizeof(Sector*), LXType_Sector, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "SectorGridColor", PIDX_SectorGridColor, offsetof(SectorTool, m_SectorGridColor), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "AsyncSectorLoading", PIDX_AsyncSectorLoading, offsetof(SectorTool, m_AsyncSectorLoading), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "NodeSize", PIDX_NodeSize, offsetof(SectorTool, m_NodeSize), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "LoadingRingSize", PIDX_LoadingRingSize, offsetof(SectorTool, m_LoadingRingSize), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX, 0,}, 
{ "ShowSectorAABBs", PIDX_ShowSectorAABBs, offsetof(SectorTool, m_ShowSectorAABBs), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ShowGrid", PIDX_ShowGrid, offsetof(SectorTool, m_ShowGrid), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "HighlightSelectedSector", PIDX_HighlightSelectedSector, offsetof(SectorTool, m_HighlightSelectedSector), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "LoadSectorsOnClick", PIDX_LoadSectorsOnClick, offsetof(SectorTool, m_LoadSectorsOnClick), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "SelectedWayIndex", PIDX_SelectedWayIndex, offsetof(SectorTool, m_SelectedWayIndex), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX, 0,}, 
};
void SectorTool::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool("Enabled", m_Enabled);
	serializer.SerializeVec3("HighlightedWorldCoordinates", m_HighlightedWorldCoordinates);
	serializer.SerializeVec2("HighlightedSectorUV", m_HighlightedSectorUV);
	serializer.SerializeObjectPtr("HighlightedSector", m_HighlightedSector);
	serializer.SerializeVec3("SectorGridColor", m_SectorGridColor);
	serializer.SerializeBool("AsyncSectorLoading", m_AsyncSectorLoading);
	serializer.SerializeFloat("NodeSize", m_NodeSize);
	serializer.SerializeInt("LoadingRingSize", m_LoadingRingSize);
	serializer.SerializeBool("ShowSectorAABBs", m_ShowSectorAABBs);
	serializer.SerializeBool("ShowGrid", m_ShowGrid);
	serializer.SerializeBool("HighlightSelectedSector", m_HighlightSelectedSector);
	serializer.SerializeBool("LoadSectorsOnClick", m_LoadSectorsOnClick);
	serializer.SerializeInt("SelectedWayIndex", m_SelectedWayIndex);
}
bool SectorTool::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool SectorTool::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowBool("Enabled", m_Enabled  );
	ImguiHelpers::ShowVec3("HighlightedWorldCoordinates", m_HighlightedWorldCoordinates , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowVec2("HighlightedSectorUV", m_HighlightedSectorUV , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowObjectPtr("HighlightedSector", m_HighlightedSector  );
	ImguiHelpers::ShowVec3("SectorGridColor", m_SectorGridColor , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowBool("AsyncSectorLoading", m_AsyncSectorLoading  );
	ImguiHelpers::ShowFloat("NodeSize", m_NodeSize , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowInt("LoadingRingSize", m_LoadingRingSize , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowBool("ShowSectorAABBs", m_ShowSectorAABBs  );
	ImguiHelpers::ShowBool("ShowGrid", m_ShowGrid  );
	ImguiHelpers::ShowBool("HighlightSelectedSector", m_HighlightSelectedSector  );
	ImguiHelpers::ShowBool("LoadSectorsOnClick", m_LoadSectorsOnClick  );
	ImguiHelpers::ShowInt("SelectedWayIndex", m_SelectedWayIndex , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	return true;
}
const char* SectorTool::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE SectorTool

SectorTool::SectorTool()
{
	
}


glm::vec3 SectorTool::GetHighlightColor(Sector* sector)
{
	static glm::vec3 m_ColorNullSector	= glm::vec3(0, 1, 0);
	static glm::vec3 m_ColorUnloadedSector = glm::vec3(0, 1, 1);
	static glm::vec3 m_ColorLoadedSector = glm::vec3(0, 0, 1);

	glm::vec3 color;
	if (sector == nullptr)
	{
		color = m_ColorNullSector;
	}
	else
	{
		if (sector->GetDataLoaded())
		{
			color = m_ColorLoadedSector;
		}
		else
		{
			color = m_ColorUnloadedSector;
		}
	}

	return color;
}


bool SectorTool::Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance)
{
	if (!m_Enabled)
	{
		return false;
	}

	Renderer* renderer = LigumX::GetInstance().GetRenderer();

	glm::vec3 worldPosition = g_Editor->GetPickingTool()->GetAimingWorldPosition();

	glm::ivec2 normalizedSectorIndex = Sector::GetNormalizedSectorIndex(glm::vec2(worldPosition));

	glm::vec2 earthStartCoords = Sector::GetStartPosition(glm::vec2(worldPosition));
	glm::vec2 worldStartCoords = Sector::EarthToWorld(earthStartCoords);

	World* world = LigumX::GetInstance().GetWorld();

	m_HighlightedSector = world->GetSectorByWorldPosition(worldPosition);

	m_HighlightedWorldCoordinates = worldPosition;
	if (m_HighlightedSector)
	{
		m_HighlightedSectorUV = m_HighlightedSector->GetUVForWorldPosition(worldPosition);
	}

	g_DefaultObjects->DefaultManipulatorEntity->SetPosition(m_HighlightedWorldCoordinates);


	if (m_HighlightSelectedSector)
	{
		int numSectorsPerSide = 2 * m_LoadingRingSize - 1;
		int offset = m_LoadingRingSize - 1;

		float scale = g_EngineSettings->GetWorldScale();

		for (int i = 0; i < numSectorsPerSide; ++i)
		{
			for (int j = 0; j < numSectorsPerSide; ++j)
			{
				glm::ivec3 offsets = glm::ivec3(i - offset, j - offset, 0);
				AABB aabb = AABB::BuildFromStartPointAndScale(glm::vec3(worldStartCoords, 0) - scale * (glm::vec3) offsets, glm::vec3(scale, scale, 1.f));

				glm::vec3 aabbColor = GetHighlightColor(m_HighlightedSector);
				LigumX::GetInstance().m_RenderDataManager->AddAABBJob(aabb, aabbColor);
			}
		}
	}

	bool canSendRequest = m_LoadSectorsOnClick && mouseButton1Down /*&& m_Request.Ready()*/;
	if (canSendRequest)
	{
		g_SectorManager->LoadSectors(m_LoadingRingSize, earthStartCoords, worldStartCoords, normalizedSectorIndex);
	}

	return true;
}


void SectorTool::DebugDisplay()
{
	if (!m_Enabled)
	{
		return;
	}

	World* world = LigumX::GetInstance().GetWorld();
	Renderer* renderer = LigumX::GetInstance().GetRenderer();

	const std::vector<Sector*> sectors = world->GetSectors();

	for (Sector* sector : g_RenderDataManager->GetVisibleSectors())
	{
		bool dataLoaded = sector->GetDataLoaded();

		if (m_ShowSectorAABBs)
		{
			const float& worldScale = g_EngineSettings->GetWorldScale() * 0.99f;
			AABB bb = AABB::BuildFromStartPointAndScale(sector->GetWorldPosition(), glm::vec3(worldScale, worldScale, 3.f));

			glm::vec3 color(0.863f, 0.078f, 0.235f);
			if (dataLoaded)
			{
				color = glm::vec3(0.255, 0.412, 0.882);
			}

			AABBJob job;
			job.m_AABB = bb;
			job.m_Color = color;

			renderer->GetRenderDataManager()->GetAABBJobs().push_back(job);
		}
	}

	if (m_ShowGrid)
	{
		renderer->RenderGrid();
	}
}


bool SectorTool::HandleMouseScroll(const glm::vec2& scrolling)
{
	bool ctrlHeld = g_InputHandler->GetCtrlHeld();

	if (ctrlHeld)
	{
		m_LoadingRingSize += scrolling.y;
		return true;
	}

	return false;
}
