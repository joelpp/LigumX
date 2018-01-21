#include "SectorTool.h"
#include "LigumX.h"
#include "Renderer.h"
#include "MainWindow.h"
#include "EngineSettings.h"
#include "RenderDataManager.h"
#include "Sector.h"
#include "SectorData.h"
#include "World.h"

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
{ "SectorLoadingOffset", PIDX_SectorLoadingOffset, offsetof(SectorTool, m_SectorLoadingOffset), 0, LXType_glmivec2, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
};
bool SectorTool::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE SectorTool


bool SectorTool::Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance)
{
	Renderer* renderer = LigumX::GetInstance().GetRenderer();
	const glm::vec2& windowSize = glm::vec2(renderer->m_Window->GetSize());

	const glm::vec2 mouseNDC = mousePosition / windowSize;
	const glm::vec2 mouseScreen = mouseNDC * 2.f - glm::vec2(1, 1);

	const glm::vec4 screenSpaceRay = glm::normalize(glm::vec4(mouseScreen, 1.f, 0.f));

	const glm::mat4 cameraInverse = glm::inverse(renderer->GetDebugCamera()->GetViewProjectionMatrix());
	const glm::vec4 worldSpaceRay = glm::normalize(glm::mul(cameraInverse, screenSpaceRay));

	if (mouseButton1Down && dragDistance != glm::vec2(0, 0))
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

	{
		g_DefaultObjects->DefaultManipulatorEntity->SetPosition(worldPosition);

		glm::vec2 startPosition = (glm::vec2)worldPosition;
		float scale = g_EngineSettings->GetWorldScale() / 20.f;

		AABB aabb;
		aabb.SetOffset(glm::vec3(startPosition - glm::vec2(scale) / 2.f, 0));

		aabb.SetScale(glm::vec3(scale, scale, 1.f));

		LigumX::GetInstance().renderData->AddAABBJob(aabb);
	}



	World* world = LigumX::GetInstance().getWorld();

	if (m_Request.Finished())
	{
		curlThread.join();


		m_LoadingSector->loadData(&m_Request, SectorData::EOSMDataType::MAP);

		RenderDataManager::CreateWaysLines(world->sectors.back());

		m_Request.Reset();
	}
	else if (mouseButton1Down && m_Request.Ready())
	{
		//glm::ivec2 cornerIndex = g_EngineSettings->GetStartLonLat * 1e7;

		glm::vec2 startCoords = Sector::GetStartPosition(glm::vec2(worldPosition));
		glm::vec2 extent = glm::vec2(g_EngineSettings->GetExtent());
		glm::vec2 normalizedSectorIndex = Sector::GetNormalizedSectorIndex(glm::vec2(worldPosition));

		//bool sectorAlreadyLoaded = false;

		//for (Sector* sector : world->sectors)
		//{
		//	if (fuzzyEquals(sector->GetPosition(), startCoords, 1e-2))
		//	{
		//		sectorAlreadyLoaded = sector->GetDataLoaded();
		//		break;
		//	}
		//}

		//if (!sectorAlreadyLoaded)
		{
			m_Request = CurlRequest(startCoords, extent);
			m_Request.Initialize();

			PRINTVEC2(normalizedSectorIndex);

			SetSectorLoadingOffset(glm::ivec2(normalizedSectorIndex));

			Sector* sector = new Sector(&m_Request);
			sector->SetOffsetIndex(glm::vec2(GetSectorLoadingOffset()));
			m_LoadingSector = sector;

			world->GetSectors().push_back(m_LoadingSector);
			world->sectors.push_back(m_LoadingSector);

			curlThread = std::thread(&CurlRequest::Execute, &m_Request);
		}
	}


	return true;
}