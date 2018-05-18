#include "LigumX.h"
#include "Editor.h"
#include "InputHandler.h"
#include "StringUtils.h"

#include "DefaultObjects.h"
#include "Node.h"

#include "Entity.h"

#include "SectorTool.h"
#include "PickingTool.h"
#include "SectorManager.h"
#include "Sector.h"
#include "SectorGraphicalData.h"
#include "Way.h"
#include "World.h"

#include "RenderDataManager.h"
#include "OSMDataProcessor.h"

#pragma region  CLASS_SOURCE OSMTool

#include "OSMTool.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData OSMTool::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(OSMTool, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", PIDX_Name, offsetof(OSMTool, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "Enabled", PIDX_Enabled, offsetof(OSMTool, m_Enabled), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "SelectedNode", PIDX_SelectedNode, offsetof(OSMTool, m_SelectedNode), 0, LXType_Node, true, LXType_None, 0, 0, 0, }, 
{ "SelectedWays", PIDX_SelectedWays, offsetof(OSMTool, m_SelectedWays), 0, LXType_stdvector, false, LXType_Way, 0, 0, 0, }, 
{ "SearchOnlyWithinSector", PIDX_SearchOnlyWithinSector, offsetof(OSMTool, m_SearchOnlyWithinSector), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "SelectedSectorIndex", PIDX_SelectedSectorIndex, offsetof(OSMTool, m_SelectedSectorIndex), 0, LXType_glmivec2, false, LXType_None, 0, 0, 0, }, 
{ "WorldSpacePosition", PIDX_WorldSpacePosition, offsetof(OSMTool, m_WorldSpacePosition), 0, LXType_glmvec3, false, LXType_None, 0, 0, 0, }, 
{ "ShowNodes", PIDX_ShowNodes, offsetof(OSMTool, m_ShowNodes), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowWays", PIDX_ShowWays, offsetof(OSMTool, m_ShowWays), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowFlatWays", PIDX_ShowFlatWays, offsetof(OSMTool, m_ShowFlatWays), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "DebugPointInRoad", PIDX_DebugPointInRoad, offsetof(OSMTool, m_DebugPointInRoad), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
};
bool OSMTool::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE OSMTool

OSMTool::OSMTool()
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


bool OSMTool::Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance)
{
	if (mouseButton1Down)
	{
		m_WorldSpacePosition = g_Editor->GetPickingTool()->GetAimingWorldPosition();

		glm::ivec2 normalizedSectorIndex = Sector::GetNormalizedSectorIndex(glm::vec2(m_WorldSpacePosition));

		Node* node = nullptr;

		if (m_DebugPointInRoad)
		{
			Sector* sector = g_World->GetSectorByIndex(normalizedSectorIndex);
			bool inRoad = PointInRoad(sector, m_WorldSpacePosition);

			Material* material = g_DefaultObjects->DefaultManipulatorEntity->GetModel()->GetMaterials()[0];
			g_DefaultObjects->DefaultManipulatorEntity->SetPosition(m_WorldSpacePosition);

			glm::vec3 diffuseColor = inRoad ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0);
			material->SetDiffuseColor(diffuseColor);
			
			return true;
		}
		else
		{
			node = g_SectorManager->GetClosestNode(glm::vec2(m_WorldSpacePosition), m_SearchOnlyWithinSector);
			g_DefaultObjects->DefaultManipulatorEntity->SetPosition(node->GetWorldPosition());
		}


		m_SelectedNode = node;


		for (Way* newWay : m_SelectedNode->GetWays())
		{
			bool found = false;
			for (Way* oldWay : m_SelectedWays)
			{
				if (newWay == oldWay)
				{
					found = true;
					break;
				}

			}

			if (!newWay->GetName().empty())
			{
				std::stringstream ss;
				ss << glm::to_string(g_InputHandler->GetMousePosition()) << " " << newWay->GetName();
				g_RenderDataManager->AddMouseMessage(ss.str());
			}
		}


		m_SelectedWays = std::vector<Way*>(m_SelectedNode->GetWays());


		m_SelectedSectorIndex = normalizedSectorIndex;
		return true;

	}
	return false;
}

void OSMTool::DisplaySectorDebug(Sector* sector)
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
		int selectedWay = 0;

		if (sector->GetOffsetIndex() == GetSelectedSectorIndex() && GetSelectedWays().size() > 0)
		{
			selectedWay = GetSelectedWays()[0]->GetIndexInSector();
		}

		renderer->RenderDebugWays(gfxData->GetWaysModel(), identity, renderer->pPipelineLines, m_WayDisplayToggles, m_WayDebugColors, selectedWay);
	}
}

void OSMTool::DebugDisplay()
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
	
	}
}
