#include "SectorTool.h"
#include "LigumX.h"
#include "Renderer.h"
#include "MainWindow.h"
#include "EngineSettings.h"
#include "RenderDataManager.h"
#include "SectorManager.h"
#include "Sector.h"
#include "SectorData.h"
#include "SectorGraphicalData.h"
#include "World.h"
#include "GUI.h"
#include "Editor.h"
#include "RenderDataManager.h"

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
{ "ShowNodes", PIDX_ShowNodes, offsetof(SectorTool, m_ShowNodes), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowWays", PIDX_ShowWays, offsetof(SectorTool, m_ShowWays), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowFlatWays", PIDX_ShowFlatWays, offsetof(SectorTool, m_ShowFlatWays), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowSectorAABBs", PIDX_ShowSectorAABBs, offsetof(SectorTool, m_ShowSectorAABBs), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowGrid", PIDX_ShowGrid, offsetof(SectorTool, m_ShowGrid), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "HighlightSelectedSector", PIDX_HighlightSelectedSector, offsetof(SectorTool, m_HighlightSelectedSector), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "LoadSectorsOnClick", PIDX_LoadSectorsOnClick, offsetof(SectorTool, m_LoadSectorsOnClick), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "a", PIDX_a, offsetof(SectorTool, m_a), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowHIGHWAYTRUNK", PIDX_ShowHIGHWAYTRUNK, offsetof(SectorTool, m_ShowHIGHWAYTRUNK), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowHIGHWAYPRIMARY", PIDX_ShowHIGHWAYPRIMARY, offsetof(SectorTool, m_ShowHIGHWAYPRIMARY), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowHIGHWAYSECONDARY", PIDX_ShowHIGHWAYSECONDARY, offsetof(SectorTool, m_ShowHIGHWAYSECONDARY), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowHIGHWAYTERTIARY", PIDX_ShowHIGHWAYTERTIARY, offsetof(SectorTool, m_ShowHIGHWAYTERTIARY), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowHIGHWAYRESIDENTIAL", PIDX_ShowHIGHWAYRESIDENTIAL, offsetof(SectorTool, m_ShowHIGHWAYRESIDENTIAL), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowHIGHWAYUNCLASSIFIED", PIDX_ShowHIGHWAYUNCLASSIFIED, offsetof(SectorTool, m_ShowHIGHWAYUNCLASSIFIED), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowHIGHWAYSERVICE", PIDX_ShowHIGHWAYSERVICE, offsetof(SectorTool, m_ShowHIGHWAYSERVICE), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowBUILDINGUNMARKED", PIDX_ShowBUILDINGUNMARKED, offsetof(SectorTool, m_ShowBUILDINGUNMARKED), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowBUILDINGSCHOOL", PIDX_ShowBUILDINGSCHOOL, offsetof(SectorTool, m_ShowBUILDINGSCHOOL), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowBUILDINGADDRINTERP", PIDX_ShowBUILDINGADDRINTERP, offsetof(SectorTool, m_ShowBUILDINGADDRINTERP), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowRAILWAYSUBWAY", PIDX_ShowRAILWAYSUBWAY, offsetof(SectorTool, m_ShowRAILWAYSUBWAY), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowNATURALWOOD", PIDX_ShowNATURALWOOD, offsetof(SectorTool, m_ShowNATURALWOOD), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowNATURALWATER", PIDX_ShowNATURALWATER, offsetof(SectorTool, m_ShowNATURALWATER), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowLEISUREPARK", PIDX_ShowLEISUREPARK, offsetof(SectorTool, m_ShowLEISUREPARK), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowADDRINTERPOLATION", PIDX_ShowADDRINTERPOLATION, offsetof(SectorTool, m_ShowADDRINTERPOLATION), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowGRIDLINE", PIDX_ShowGRIDLINE, offsetof(SectorTool, m_ShowGRIDLINE), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowaDEBUG", PIDX_ShowaDEBUG, offsetof(SectorTool, m_ShowaDEBUG), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowLANDUSE", PIDX_ShowLANDUSE, offsetof(SectorTool, m_ShowLANDUSE), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowBOUNDARY", PIDX_ShowBOUNDARY, offsetof(SectorTool, m_ShowBOUNDARY), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowCONTOUR", PIDX_ShowCONTOUR, offsetof(SectorTool, m_ShowCONTOUR), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "WayDisplayFlags", PIDX_WayDisplayFlags, offsetof(SectorTool, m_WayDisplayFlags), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
};
bool SectorTool::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE SectorTool


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
	Renderer* renderer = LigumX::GetInstance().GetRenderer();

	glm::vec3 worldPosition = GetAimingWorldSpacePosition(mousePosition);

	glm::ivec2 normalizedSectorIndex = Sector::GetNormalizedSectorIndex(glm::vec2(worldPosition));

	glm::vec2 earthStartCoords = Sector::GetStartPosition(glm::vec2(worldPosition));
	glm::vec2 worldStartCoords = Sector::EarthToWorld(earthStartCoords);

	g_DefaultObjects->DefaultManipulatorEntity->SetPosition(worldPosition);

	World* world = LigumX::GetInstance().GetWorld();

	m_HighlightedSector = world->GetSectorByIndex(normalizedSectorIndex);
	m_HighlightedWorldCoordinates = worldPosition;

	if (m_HighlightSelectedSector)
	{
		float scale = g_EngineSettings->GetWorldScale();
		AABB aabb = AABB::BuildFromStartPointAndScale(glm::vec3(worldStartCoords, 0), glm::vec3(scale, scale, 1.f));

		glm::vec3 aabbColor = GetHighlightColor(m_HighlightedSector);
		LigumX::GetInstance().m_RenderDataManager->AddAABBJob(aabb, aabbColor);
	}

	bool canSendRequest = m_LoadSectorsOnClick && mouseButton1Down && m_Request.Ready() && (!m_HighlightedSector || !m_HighlightedSector->GetDataLoaded());
	if (canSendRequest)
	{
		m_Request = CurlRequest(earthStartCoords, glm::vec2(g_EngineSettings->GetExtent()), m_AsyncSectorLoading);
		m_Request.SetSectorIndex(normalizedSectorIndex);
		m_Request.Initialize();
		m_Request.Start();
	}
	else if (m_Request.Finished())
	{
		m_Request.End();

		g_SectorManager->LoadRequest(&m_Request, SectorData::EOSMDataType::MAP);

		RenderDataManager::CreateWaysLines(m_Request.GetSector());

		m_Request.GetSector()->SetDataLoaded(true);

		m_Request.Reset();
	}

	return true;
}


void SectorTool::DisplaySectorDebug(SectorGraphicalData* gfxData)
{
	Renderer* renderer = LigumX::GetInstance().GetRenderer();

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
		m_WayDisplayFlags = 0;
		m_WayDisplayFlags |= m_ShowHIGHWAYTRUNK			? OSMElement::HIGHWAY_TRUNK			: 0;
		m_WayDisplayFlags |= m_ShowHIGHWAYPRIMARY		? OSMElement::HIGHWAY_PRIMARY		: 0;
		m_WayDisplayFlags |= m_ShowHIGHWAYSECONDARY		? OSMElement::HIGHWAY_SECONDARY		: 0;
		m_WayDisplayFlags |= m_ShowHIGHWAYTERTIARY		? OSMElement::HIGHWAY_TERTIARY		: 0;
		m_WayDisplayFlags |= m_ShowHIGHWAYRESIDENTIAL	? OSMElement::HIGHWAY_RESIDENTIAL	: 0;
		m_WayDisplayFlags |= m_ShowHIGHWAYUNCLASSIFIED	? OSMElement::HIGHWAY_UNCLASSIFIED	: 0;
		m_WayDisplayFlags |= m_ShowHIGHWAYSERVICE		? OSMElement::HIGHWAY_SERVICE		: 0;
		m_WayDisplayFlags |= m_ShowBUILDINGUNMARKED		? OSMElement::BUILDING_UNMARKED		: 0;
		m_WayDisplayFlags |= m_ShowBUILDINGSCHOOL		? OSMElement::BUILDING_SCHOOL		: 0;
		m_WayDisplayFlags |= m_ShowBUILDINGADDRINTERP	? OSMElement::BUILDING_ADDRINTERP	: 0;
		m_WayDisplayFlags |= m_ShowRAILWAYSUBWAY 		? OSMElement::RAILWAY_SUBWAY		: 0;
		m_WayDisplayFlags |= m_ShowNATURALWOOD			? OSMElement::NATURAL_WOOD			: 0;
		m_WayDisplayFlags |= m_ShowNATURALWATER			? OSMElement::NATURAL_WATER			: 0;
		m_WayDisplayFlags |= m_ShowLEISUREPARK			? OSMElement::LEISURE_PARK			: 0;
		m_WayDisplayFlags |= m_ShowADDRINTERPOLATION	? OSMElement::ADDR_INTERPOLATION	: 0;
		m_WayDisplayFlags |= m_ShowGRIDLINE				? OSMElement::GRID_LINE				: 0;
		m_WayDisplayFlags |= m_ShowaDEBUG				? OSMElement::aDEBUG				: 0;
		m_WayDisplayFlags |= m_ShowLANDUSE				? OSMElement::LANDUSE				: 0;
		m_WayDisplayFlags |= m_ShowBOUNDARY				? OSMElement::BOUNDARY				: 0;
		m_WayDisplayFlags |= m_ShowCONTOUR				? OSMElement::CONTOUR				: 0;

		
		renderer->RenderDebugWays(gfxData->GetWaysModel(), identity, renderer->pPipelineLines, m_WayDisplayFlags, 0);
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
			DisplaySectorDebug(sector->GetGraphicalData());
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
