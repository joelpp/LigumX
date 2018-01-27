#include "SectorTool.h"
#include "LigumX.h"
#include "Renderer.h"
#include "MainWindow.h"
#include "EngineSettings.h"
#include "RenderDataManager.h"
#include "SectorManager.h"
#include "Sector.h"
#include "SectorData.h"
#include "World.h"
#include "GUI.h"

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
};
bool SectorTool::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE SectorTool

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

bool SectorTool::Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance)
{
	glm::vec3 worldPosition = GetAimingWorldSpacePosition(mousePosition, mouseButton1Down && dragDistance != glm::vec2(0, 0));

	glm::vec2 earthStartCoords = Sector::GetStartPosition(glm::vec2(worldPosition));
	glm::ivec2 normalizedSectorIndex = Sector::GetNormalizedSectorIndex(glm::vec2(worldPosition));

	glm::vec2 worldStartCoords = Sector::EarthToWorld(earthStartCoords);

	g_DefaultObjects->DefaultManipulatorEntity->SetPosition(worldPosition);

	float scale = g_EngineSettings->GetWorldScale();

	World* world = LigumX::GetInstance().GetWorld();

	m_HighlightedSector = world->GetSectorByIndex(normalizedSectorIndex);
	m_HighlightedWorldCoordinates = worldPosition;

	AABB aabb = AABB::BuildFromStartPointAndScale(glm::vec3(worldStartCoords, 0), glm::vec3(scale, scale, 1.f));

	glm::vec3 aabbColor = GetHighlightColor(m_HighlightedSector);
	
		
	LigumX::GetInstance().renderData->AddAABBJob(aabb, aabbColor);

	bool canSendRequest = mouseButton1Down && m_Request.Ready() && (!m_HighlightedSector || !m_HighlightedSector->GetDataLoaded());
	if (canSendRequest)
	{
		m_Request = CurlRequest(earthStartCoords, glm::vec2(g_EngineSettings->GetExtent()));
		m_Request.SetSectorIndex(normalizedSectorIndex);
		m_Request.Initialize();

		Sector* sector = m_HighlightedSector;
		if (sector == nullptr)
		{
			sector = g_SectorManager->CreateSector(&m_Request);
		}
		else
		{
			m_Request.SetSector(sector);
		}

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


void SectorTool::Display()
{
	//g_GUI->BeginWindow(1000, 700, 0, 0, "Sector Tool");



	//g_GUI->EndWindow();
}
