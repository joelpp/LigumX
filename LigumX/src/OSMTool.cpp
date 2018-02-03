#include "Editor.h"
#include "SectorTool.h"
#include "InputHandler.h"
#include "SectorManager.h"
#include "DefaultObjects.h"
#include "Node.h"
#include "Entity.h"

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
};
bool OSMTool::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE OSMTool


bool OSMTool::Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance)
{
	if (mouseButton1Down)
	{
		const glm::vec2& mousePosition = g_InputHandler->GetMousePosition();

		glm::vec3 wsPosition = g_Editor->GetSectorTool()->GetAimingWorldSpacePosition(mousePosition);

		Node* node = g_SectorManager->GetClosestNode(glm::vec2(wsPosition));

		g_DefaultObjects->DefaultManipulatorEntity->SetPosition(node->GetWorldPosition());

		m_SelectedNode = node;

		m_SelectedWays = std::vector<Way*>(m_SelectedNode->GetWays());
		return true;

	}
	return false;
}
