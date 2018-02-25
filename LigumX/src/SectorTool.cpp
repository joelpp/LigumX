#include "SectorTool.h"
#include "LigumX.h"
#include "EngineSettings.h"

#include "Renderer.h"
#include "MainWindow.h"
#include "RenderDataManager.h"
#include "GUI.h"

#include "Editor.h"
#include "OSMTool.h"

#include "World.h"
#include "Sector.h"
#include "SectorManager.h"
#include "SectorData.h"
#include "SectorGraphicalData.h"
#include "Way.h"

#pragma region  CLASS_SOURCE SectorTool

#include "SectorTool.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData SectorTool::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(SectorTool, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", PIDX_Name, offsetof(SectorTool, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "Enabled", PIDX_Enabled, offsetof(SectorTool, m_Enabled), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "HighlightedWorldCoordinates", PIDX_HighlightedWorldCoordinates, offsetof(SectorTool, m_HighlightedWorldCoordinates), 0, LXType_glmvec3, false, LXType_None, 0, 0, 0, }, 
{ "HighlightedSector", PIDX_HighlightedSector, offsetof(SectorTool, m_HighlightedSector), 0, LXType_Sector, true, LXType_None, 0, 0, 0, }, 
{ "SectorGridColor", PIDX_SectorGridColor, offsetof(SectorTool, m_SectorGridColor), 0, LXType_glmvec3, false, LXType_None, 0, 0, 0, }, 
{ "AsyncSectorLoading", PIDX_AsyncSectorLoading, offsetof(SectorTool, m_AsyncSectorLoading), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "NodeSize", PIDX_NodeSize, offsetof(SectorTool, m_NodeSize), 0, LXType_float, false, LXType_None, 0, 0, 0, }, 
{ "LoadingRingSize", PIDX_LoadingRingSize, offsetof(SectorTool, m_LoadingRingSize), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "ShowNodes", PIDX_ShowNodes, offsetof(SectorTool, m_ShowNodes), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowWays", PIDX_ShowWays, offsetof(SectorTool, m_ShowWays), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowFlatWays", PIDX_ShowFlatWays, offsetof(SectorTool, m_ShowFlatWays), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowSectorAABBs", PIDX_ShowSectorAABBs, offsetof(SectorTool, m_ShowSectorAABBs), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowGrid", PIDX_ShowGrid, offsetof(SectorTool, m_ShowGrid), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "HighlightSelectedSector", PIDX_HighlightSelectedSector, offsetof(SectorTool, m_HighlightSelectedSector), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "LoadSectorsOnClick", PIDX_LoadSectorsOnClick, offsetof(SectorTool, m_LoadSectorsOnClick), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "SelectedWayIndex", PIDX_SelectedWayIndex, offsetof(SectorTool, m_SelectedWayIndex), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
};
bool SectorTool::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE SectorTool

SectorTool::SectorTool()
{
	m_WayDisplayToggles.resize(OSMElementType_Count);

	for (int i = 0; i < OSMElementType_Count; ++i)
	{
		m_WayDisplayToggles[i] = true;
	}

	m_WayDebugColors =
	{
		vec3(1.0, 1.0, 1.0),	// HighwayTrunk,
		vec3(0.9, 0.9, 0.9),	// HighwayPrimary,
		vec3(0.8, 0.8, 0.8),	// HighwaySecondary,
		vec3(0.7, 0.7, 0.7),	// HighwayTertiary,
		vec3(0.6, 0.6, 0.6),	// HighwayResidential,
		vec3(0.5, 0.5, 0.5),	// HighwayService,
		vec3(0.4, 0.4, 0.4),	// HighwayUnclassified,
		vec3(1.0, 0.0, 1.0),	// Sidewalk,
		vec3(1.0, 1.0, 1.0),	// Contour,
		vec3(0.0, 0.0, 1.0),	// Building_Unmarked,
		vec3(0.0, 0.0, 1.0),	// Building_School,
		vec3(0.0, 0.5, 0.0),	// Building_Addressinterpolation,
		vec3(0.0, 0.0, 0.5),	// Boundary,
		vec3(0.1, 1.0, 0.1),	// LeisurePark,
		vec3(0.0, 1.0, 0.0),	// NaturalWood,
		vec3(0.0, 0.2, 0.8),	// NaturalWater,
		vec3(1.0, 1.0, 1.0),	// Landuse,
		vec3(1.0, 1.0, 1.0),	// RailwaySubway,
		vec3(0.0, 0.8, 1.0),	// AddressInterpolation,
		vec3(1.0, 1.0, 1.0),	// NotImplemented,
		vec3(1.0, 0.1, 0.1)		// Unknown,
	};
}


glm::vec3 SectorTool::GetWorldSpaceRay(glm::vec3 ndc, const glm::mat4& projectionMatrixInverse, const glm::mat4& viewMatrixInverse)
{
	glm::vec4 clipCoords = glm::vec4(glm::vec2(ndc), -1.0f, 1.0f);
	clipCoords.y = -clipCoords.y;

	glm::vec4 cameraRay = projectionMatrixInverse * clipCoords;

	cameraRay.z = -1;
	cameraRay.w = 0;

	glm::vec4 worldSpaceRay = viewMatrixInverse * cameraRay;

	return glm::normalize(glm::vec3(worldSpaceRay));
}

glm::vec3 SectorTool::GetAimingWorldSpacePosition2(glm::vec3 worldSpaceRay, glm::vec3 cameraPosition)
{
	glm::vec3 planeNormal = glm::vec3(0, 0, 1);
	glm::vec3 pointOnPlane = glm::vec3(0, 0, 0);

	float t = dot(pointOnPlane - cameraPosition, planeNormal) / glm::dot(glm::vec3(worldSpaceRay), planeNormal);

	glm::vec3 worldPosition = cameraPosition + t * glm::vec3(worldSpaceRay);

	return worldPosition;
}


glm::vec3 SectorTool::GetAimingWorldSpacePosition(const glm::vec2& mouseScreenPosition, bool printDebugInfo)
{
	Renderer* renderer = LigumX::GetInstance().GetRenderer();

	const glm::vec2& windowSize = glm::vec2(renderer->m_Window->GetSize());

	const glm::vec2 mouseNDC = mouseScreenPosition / windowSize;
	const glm::vec2 mouseScreen = mouseNDC * 2.f - glm::vec2(1, 1);

	const glm::vec4 screenSpaceRay = glm::normalize(glm::vec4(mouseScreen, 1.f, 0.f));

	const glm::mat4 cameraInverse = glm::inverse(renderer->GetDebugCamera()->GetViewProjectionMatrix());
	const glm::vec4 worldSpaceRay = glm::normalize(glm::mul(cameraInverse, screenSpaceRay));

	if (printDebugInfo)
	{
		PRINTVEC2(mouseNDC);
		PRINTVEC2(mouseScreen);

		PRINTVEC4(worldSpaceRay);
	}

	const glm::vec3& cameraPosition = renderer->GetDebugCamera()->GetPosition();
	const glm::vec3& cameraFront = renderer->GetDebugCamera()->GetFrontVector();

	const glm::vec3 planeNormal = glm::vec3(0, 0, 1);
	const glm::vec3 pointOnPlane = glm::vec3(0, 0, 0);

	float t = glm::dot(pointOnPlane - cameraPosition, planeNormal) / glm::dot(cameraFront, planeNormal);

	glm::vec3 worldPosition = cameraPosition + t * cameraFront;

	return worldPosition;
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

glm::vec3 SectorTool::GetAimingWorldSpacePosition(const glm::vec2& mousePosition)
{
	Renderer* renderer = LigumX::GetInstance().GetRenderer();

	glm::ivec2 screenSize = renderer->m_Window->GetSize();

	glm::vec2 normalizedScreenPosition = glm::vec2(mousePosition / glm::vec2(screenSize));

	glm::vec3 ndc = glm::vec3(2.f * normalizedScreenPosition - glm::vec2(1.f, 1.f), 1.f);

	glm::vec3 wsRay = GetWorldSpaceRay(ndc, renderer->GetDebugCamera()->GetProjectionMatrixInverse(), renderer->GetDebugCamera()->GetViewMatrixInverse());

	glm::vec3 worldPosition = GetAimingWorldSpacePosition2(wsRay, renderer->GetDebugCamera()->GetPosition());

	return worldPosition;
}

bool SectorTool::Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance)
{
	if (!m_Enabled)
	{
		return false;
	}

	Renderer* renderer = LigumX::GetInstance().GetRenderer();

	glm::vec3 worldPosition = GetAimingWorldSpacePosition(mousePosition);

	glm::ivec2 normalizedSectorIndex = Sector::GetNormalizedSectorIndex(glm::vec2(worldPosition));

	glm::vec2 earthStartCoords = Sector::GetStartPosition(glm::vec2(worldPosition));
	glm::vec2 worldStartCoords = Sector::EarthToWorld(earthStartCoords);

	World* world = LigumX::GetInstance().GetWorld();

	m_HighlightedSector = world->GetSectorByIndex(normalizedSectorIndex);

	if (m_HighlightedSector != nullptr)
	{
		float height = m_HighlightedSector->SampleHeight(worldPosition);
		worldPosition.z = height;
	}

	m_HighlightedWorldCoordinates = worldPosition;

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


	bool canSendRequest = m_LoadSectorsOnClick && mouseButton1Down && m_Request.Ready();
	if (canSendRequest)
	{
		int numSectorsPerSide = 2 * m_LoadingRingSize - 1;
		int offset = m_LoadingRingSize - 1;

		for (int i = 0; i < numSectorsPerSide; ++i)
		{
			for (int j = 0; j < numSectorsPerSide; ++j)
			{
				glm::ivec2 offsets = glm::ivec2(i - offset, j - offset);
				glm::vec2 earthExtent = glm::vec2(g_EngineSettings->GetExtent());
				m_Request = CurlRequest(earthStartCoords - earthExtent * (glm::vec2) offsets, earthExtent, m_AsyncSectorLoading);

				glm::ivec2 requestedSectorIndex = normalizedSectorIndex + glm::ivec2(offsets);
				m_Request.SetSectorIndex(requestedSectorIndex);

				Sector* requestSector = world->GetSectorByIndex(m_Request.GetSectorIndex());
				if (!requestSector)
				{
					requestSector = g_SectorManager->CreateSector(requestedSectorIndex);
				}
				m_Request.SetSector(requestSector);

				if (requestSector->GetDataLoaded())
				{
					continue;
				}

				m_Request.Initialize();
				m_Request.Start();
				// todo : reenable threading ( it was gone for a while anyway)
				//}
				//else if (m_Request.Finished())
				//{
				m_Request.End();

				g_SectorManager->LoadRequest(&m_Request, SectorData::EOSMDataType::MAP);

				RenderDataManager::CreateWaysLines(m_Request.GetSector());

				m_Request.GetSector()->SetDataLoaded(true);

				m_Request.Reset();
			}
		}


		
	}

	return true;
}


void SectorTool::DisplaySectorDebug(Sector* sector)
{
	Renderer* renderer = LigumX::GetInstance().GetRenderer();
	SectorGraphicalData* gfxData = sector->GetGraphicalData();

	glm::mat4 identity = glm::mat4(1.0);

	if (m_ShowNodes)
	{
		renderer->RenderDebugModel(gfxData->GetNodesModel(), identity, renderer->pPipelineNodes);
	}

	if (m_ShowWays) 
	{
		for (Model* wayModel : gfxData->GetWaysModelsVector())
		{
			renderer->RenderDebugModel(wayModel, identity, renderer->pPipelineLines);
		}
	}

	if (m_ShowFlatWays)
	{	
		OSMTool* osmTool = g_Editor->GetOSMTool();

		int selectedWay = 0;

		if (sector->GetOffsetIndex() == osmTool->GetSelectedSectorIndex() && osmTool->GetSelectedWays().size() > 0)
		{
			selectedWay = osmTool->GetSelectedWays()[0]->GetIndexInSector();
		}

		renderer->RenderDebugWays(gfxData->GetWaysModel(), identity, renderer->pPipelineLines, m_WayDisplayToggles, m_WayDebugColors, selectedWay);
	}
}


void SectorTool::Display()
{
	if (!m_Enabled)
	{
		return;
	}

	World* world = LigumX::GetInstance().GetWorld();
	Renderer* renderer = LigumX::GetInstance().GetRenderer();

	const std::vector<Sector*> sectors = world->GetSectors();

	for (Sector* sector : world->GetSectors())
	{
		bool dataLoaded = sector->GetDataLoaded();
		if (dataLoaded)
		{
			DisplaySectorDebug(sector);
		}

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
