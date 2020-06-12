#include "LigumX.h"
#include "Editor.h"
#include "InputHandler.h"
#include "EngineSettings.h"
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
#include "Visual.h"

#pragma region  CLASS_SOURCE OSMTool

#include "OSMTool.h"
#include "serializer.h"
#include "Node.h"
#include "Way.h"
#include "OSMElementComponent.h"
const ClassPropertyData OSMTool::g_Properties[] = 
{
{ "Enabled", PIDX_Enabled, offsetof(OSMTool, m_Enabled), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "SelectedNode", PIDX_SelectedNode, offsetof(OSMTool, m_SelectedNode), 0, LXType_ObjectPtr, sizeof(Node*), LXType_Node, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "SelectedWays", PIDX_SelectedWays, offsetof(OSMTool, m_SelectedWays), 0, LXType_stdvector, sizeof(std::vector<Way*>), LXType_stdvector, false, LXType_Way, true, 0, 0, 0, 0,}, 
{ "SelectedOSMElementComponent", PIDX_SelectedOSMElementComponent, offsetof(OSMTool, m_SelectedOSMElementComponent), 0, LXType_ObjectPtr, sizeof(OSMElementComponent*), LXType_OSMElementComponent, true, LXType_None, false, PropertyFlags_Transient, 0, 0, 0,}, 
{ "SearchOnlyWithinSector", PIDX_SearchOnlyWithinSector, offsetof(OSMTool, m_SearchOnlyWithinSector), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "SelectedSectorIndex", PIDX_SelectedSectorIndex, offsetof(OSMTool, m_SelectedSectorIndex), 0, LXType_glmivec2, sizeof(glm::ivec2), LXType_glmivec2, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "WorldSpacePosition", PIDX_WorldSpacePosition, offsetof(OSMTool, m_WorldSpacePosition), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "ShowNodes", PIDX_ShowNodes, offsetof(OSMTool, m_ShowNodes), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ShowWays", PIDX_ShowWays, offsetof(OSMTool, m_ShowWays), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ShowFlatWays", PIDX_ShowFlatWays, offsetof(OSMTool, m_ShowFlatWays), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "DebugPointInRoad", PIDX_DebugPointInRoad, offsetof(OSMTool, m_DebugPointInRoad), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "COMMAND_LoadAllDebugWays", PIDX_COMMAND_LoadAllDebugWays, offsetof(OSMTool, m_COMMAND_LoadAllDebugWays), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, PropertyFlags_SetCallback | PropertyFlags_Transient, 0, 0, WriteSetFunction(OSMTool, COMMAND_LoadAllDebugWays, bool),}, 
{ "COMMAND_CreateAllEntityModels", PIDX_COMMAND_CreateAllEntityModels, offsetof(OSMTool, m_COMMAND_CreateAllEntityModels), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, PropertyFlags_SetCallback | PropertyFlags_Transient, 0, 0, WriteSetFunction(OSMTool, COMMAND_CreateAllEntityModels, bool),}, 
};
void OSMTool::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool(g_Properties[PIDX_Enabled], m_Enabled);
	serializer.SerializeObjectPtr(g_Properties[PIDX_SelectedNode], m_SelectedNode);
	serializer.SerializeVector(g_Properties[PIDX_SelectedWays], m_SelectedWays);
	serializer.SerializeBool(g_Properties[PIDX_SearchOnlyWithinSector], m_SearchOnlyWithinSector);
	serializer.SerializeIVec2(g_Properties[PIDX_SelectedSectorIndex], m_SelectedSectorIndex);
	serializer.SerializeVec3(g_Properties[PIDX_WorldSpacePosition], m_WorldSpacePosition);
	serializer.SerializeBool(g_Properties[PIDX_ShowNodes], m_ShowNodes);
	serializer.SerializeBool(g_Properties[PIDX_ShowWays], m_ShowWays);
	serializer.SerializeBool(g_Properties[PIDX_ShowFlatWays], m_ShowFlatWays);
	serializer.SerializeBool(g_Properties[PIDX_DebugPointInRoad], m_DebugPointInRoad);
}
bool OSMTool::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool OSMTool::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Enabled], &m_Enabled  );
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_SelectedNode], &m_SelectedNode  );
	ImguiHelpers::ShowProperty3(this, g_Properties[PIDX_SelectedWays], m_SelectedWays  );
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_SelectedOSMElementComponent], &m_SelectedOSMElementComponent  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_SearchOnlyWithinSector], &m_SearchOnlyWithinSector  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_SelectedSectorIndex], &m_SelectedSectorIndex , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_WorldSpacePosition], &m_WorldSpacePosition , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ShowNodes], &m_ShowNodes  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ShowWays], &m_ShowWays  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ShowFlatWays], &m_ShowFlatWays  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_DebugPointInRoad], &m_DebugPointInRoad  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_COMMAND_LoadAllDebugWays], &m_COMMAND_LoadAllDebugWays  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_COMMAND_CreateAllEntityModels], &m_COMMAND_CreateAllEntityModels  );
	return true;
}
void OSMTool::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	OSMTool* other = (OSMTool*) otherObj;
	other->SetEnabled(m_Enabled);
	other->SetSelectedNode(m_SelectedNode);
	other->SetSelectedWays(m_SelectedWays);
	other->SetSelectedOSMElementComponent(m_SelectedOSMElementComponent);
	other->SetSearchOnlyWithinSector(m_SearchOnlyWithinSector);
	other->SetSelectedSectorIndex(m_SelectedSectorIndex);
	other->SetWorldSpacePosition(m_WorldSpacePosition);
	other->SetShowNodes(m_ShowNodes);
	other->SetShowWays(m_ShowWays);
	other->SetShowFlatWays(m_ShowFlatWays);
	other->SetDebugPointInRoad(m_DebugPointInRoad);
	other->SetCOMMAND_LoadAllDebugWays(m_COMMAND_LoadAllDebugWays);
	other->SetCOMMAND_CreateAllEntityModels(m_COMMAND_CreateAllEntityModels);
}
const char* OSMTool::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE OSMTool

OSMTool::OSMTool()
{
	m_WayDebugColors.resize(OSMElementType_Count);
	m_WayDisplayToggles.resize(OSMElementType_Count);

	for (int i = 0; i < OSMElementType_Count; ++i)
	{
		m_WayDisplayToggles[i] = true;
	}

	m_WayDebugColors[OSMElementType_HighwayTrunk]						= glm::vec3(1.0, 1.0, 1.0);	
	m_WayDebugColors[OSMElementType_HighwayPrimary]						= glm::vec3(0.9, 0.9, 0.9);	
	m_WayDebugColors[OSMElementType_HighwaySecondary]					= glm::vec3(0.8, 0.8, 0.8);	
	m_WayDebugColors[OSMElementType_HighwayTertiary] 					= glm::vec3(0.7, 0.7, 0.7);	
	m_WayDebugColors[OSMElementType_HighwayResidential]					= glm::vec3(0.6, 0.6, 0.6);	
	m_WayDebugColors[OSMElementType_HighwayService]						= glm::vec3(0.5, 0.5, 0.5);	
	m_WayDebugColors[OSMElementType_HighwayUnclassified]				= glm::vec3(0.4, 0.4, 0.4);	
	m_WayDebugColors[OSMElementType_Sidewalk]							= glm::vec3(1.0, 0.0, 1.0);	
	m_WayDebugColors[OSMElementType_Contour]							= glm::vec3(1.0, 1.0, 1.0);	
	m_WayDebugColors[OSMElementType_Building_Unmarked]					= glm::vec3(0.0, 0.0, 1.0);	
	m_WayDebugColors[OSMElementType_Building_School]					= glm::vec3(0.0, 0.0, 1.0);	
	m_WayDebugColors[OSMElementType_Building_Addressinterpolation]		= glm::vec3(0.0, 0.5, 0.0);	
	m_WayDebugColors[OSMElementType_Boundary]							= glm::vec3(0.0, 0.0, 0.5);	
	m_WayDebugColors[OSMElementType_LeisurePark]						= glm::vec3(0.1, 1.0, 0.1);	
	m_WayDebugColors[OSMElementType_NaturalWood]						= glm::vec3(0.0, 1.0, 0.0);	
	m_WayDebugColors[OSMElementType_NaturalWater]						= glm::vec3(0.0, 0.2, 0.8);	
	m_WayDebugColors[OSMElementType_LanduseRetail]						= glm::vec3(0.86, 0.86, 0.86);
	m_WayDebugColors[OSMElementType_LanduseIndustrial]					= glm::vec3(91, 47, 82) / 255.f;
	m_WayDebugColors[OSMElementType_RailwaySubway]						= glm::vec3(0.0, 0.8, 1.0);	
	m_WayDebugColors[OSMElementType_AddressInterpolation]				= glm::vec3(1.0, 1.0, 1.0);	
	m_WayDebugColors[OSMElementType_NotImplemented]						= glm::vec3(1.0, 0.1, 0.1);		
	m_WayDebugColors[OSMElementType_Unknown]							= glm::vec3(1.0, 0.0, 0.0);
	
}


bool OSMTool::Process(bool isActiveTool, bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance)
{

	Entity* selectedEntity = g_Editor->GetPickingTool()->GetPickedEntity();
	m_SelectedOSMElementComponent = selectedEntity ? selectedEntity->GetComponent< OSMElementComponent>() : nullptr;

	if (mouseButton1Down && isActiveTool)
	{
		m_WorldSpacePosition = g_Editor->GetPickingTool()->GetAimingWorldPosition();

		glm::ivec2 normalizedSectorIndex = Sector::GetNormalizedSectorIndex(glm::vec2(m_WorldSpacePosition));

		Node* node = nullptr;

		if (m_DebugPointInRoad)
		{
			Sector* sector = g_World->GetSectorByIndex(normalizedSectorIndex);
			bool inRoad = PointInRoad(sector, m_WorldSpacePosition);

			Visual* visual = g_DefaultObjects->DefaultManipulatorEntity->GetComponent<Visual>();
			Material* material = visual->GetModel()->GetMaterials()[0];
			g_DefaultObjects->DefaultManipulatorEntity->SetPosition(m_WorldSpacePosition);

			glm::vec3 diffuseColor = inRoad ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0);
			material->SetDiffuseColor(diffuseColor);
			
			return true;
		}
		else
		{
			node = g_SectorManager->GetClosestNode(glm::vec2(m_WorldSpacePosition), m_SearchOnlyWithinSector);

			if (node)
			{
				g_DefaultObjects->DefaultManipulatorEntity->SetPosition(node->GetWorldPosition());
			}
		}

		m_SelectedNode = node;

		if (m_SelectedNode)
		{
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

				std::stringstream mouseMessage;

				if (!newWay->GetName().empty())
				{
					mouseMessage << newWay->GetName().c_str();
				}
				else
				{
					mouseMessage << "Unnamed Way";
				}

				mouseMessage << " (id=";
				mouseMessage << newWay->GetOSMId();
				mouseMessage << ", tags=";
				mouseMessage << newWay->GetAllTags();
				mouseMessage << ")";
				
				g_RenderDataManager->AddMouseMessage(mouseMessage.str().c_str());
			}

			m_SelectedWays = std::vector<Way*>(m_SelectedNode->GetWays());
			m_SelectedSectorIndex = normalizedSectorIndex;
		}

		return true;

	}
	return false;
}

void OSMTool::DisplaySectorDebug(Sector* sector)
{
	Renderer* renderer = LigumX::GetInstance().GetRenderer();
	SectorGraphicalData* gfxData = sector->GetGraphicalData();

	glm::mat4 transfo = glm::mat4(1.0);
	//transfo = glm::translate(transfo, glm::vec3(sector->GetWorldPosition(), 0));
	//transfo = glm::scale(transfo, glm::vec3(g_EngineSettings->GetWorldScale(), g_EngineSettings->GetWorldScale(), g_EngineSettings->GetWorldScale()));

	if (m_ShowNodes)
	{
		renderer->RenderDebugModel(gfxData->GetNodesModel(), transfo, renderer->pPipelineNodes);
	}

	if (m_ShowWays)
	{
		for (Model* wayModel : gfxData->GetWaysModelsVector())
		{
			renderer->RenderDebugModel(wayModel, transfo, renderer->pPipelineLines);
		}
	}

}

void OSMTool::DebugDisplay()
{
	World* world = LigumX::GetInstance().GetWorld();
	if (!m_Enabled || !world)
	{
		return;
	}

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


	if (m_ShowFlatWays)
	{
		Entity* selectedEntity = g_Editor->GetPickingTool()->GetPickedEntity();
		renderer->RenderDebugWays(glm::mat4(1.0f), renderer->pPipelineLines, m_WayDisplayToggles, m_WayDebugColors, selectedEntity);
	}
}

void OSMTool::Reset()
{
	m_SelectedWays.clear();
	m_SelectedNode = nullptr;
	m_SelectedSectorIndex = glm::ivec2(0, 0);
	m_WorldSpacePosition = glm::vec3(0, 0, 0);
}


void OSMTool::SetCOMMAND_LoadAllDebugWays_Callback(const bool& value)
{
	if (value) 
	{
		World* world = LigumX::GetInstance().GetWorld();
		if (world)
		{
			// todo jpp : wrap this in some sort (for all components of type c in world)
			for (int e = 0; e < world->GetEntities().size(); ++e)
			{
				Entity* entity = world->GetEntities()[e];

				if (entity)
				{
					OSMElementComponent* osmElementComponent = entity->GetComponent<OSMElementComponent>();
					if (osmElementComponent && (osmElementComponent->GetDebugWayModel() == nullptr))
					{
						osmElementComponent->SetCOMMAND_CreateDebugMesh(true);
					}
				}
			}
		}

	}


	m_COMMAND_LoadAllDebugWays = false;

}

void OSMTool::SetCOMMAND_CreateAllEntityModels_Callback(const bool& value)
{
	if (value)
	{
		World* world = LigumX::GetInstance().GetWorld();
		if (world)
		{
			// todo jpp : wrap this in some sort (for all components of type c in world)
			for (int e = 0; e < world->GetEntities().size(); ++e)
			{
				Entity* entity = world->GetEntities()[e];

				if (entity)
				{
					OSMElementComponent* osmElementComponent = entity->GetComponent<OSMElementComponent>();
					Visual* visual = entity->GetComponent<Visual>();
					if (osmElementComponent && (visual == nullptr))
					{
						osmElementComponent->SetCOMMAND_CreateModel(true);
					}
				}
			}
		}

	}

	m_COMMAND_CreateAllEntityModels = false;
}
