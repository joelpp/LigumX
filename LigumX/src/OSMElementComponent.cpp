#include "OSMElementComponent.h"
#include "Node.h"
#include "RenderDataManager.h"

#pragma region  CLASS_SOURCE OSMElementComponent

#include "OSMElementComponent.h"
#include "serializer.h"
#include "Way.h"
#include "Model.h"
const ClassPropertyData OSMElementComponent::g_Properties[] = 
{
{ "Way", PIDX_Way, offsetof(OSMElementComponent, m_Way), 0, LXType_ObjectPtr, sizeof(Way*), LXType_Way, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "DebugWayModel", PIDX_DebugWayModel, offsetof(OSMElementComponent, m_DebugWayModel), 0, LXType_ObjectPtr, sizeof(Model*), LXType_Model, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "COMMAND_CreateDebugMesh", PIDX_COMMAND_CreateDebugMesh, offsetof(OSMElementComponent, m_COMMAND_CreateDebugMesh), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, PropertyFlags_SetCallback, 0, 0, WriteSetFunction(OSMElementComponent, COMMAND_CreateDebugMesh, bool),}, 
};
void OSMElementComponent::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeObjectPtr(g_Properties[PIDX_Way], m_Way);
	serializer.SerializeObjectPtr(g_Properties[PIDX_DebugWayModel], m_DebugWayModel);
	serializer.SerializeBool(g_Properties[PIDX_COMMAND_CreateDebugMesh], m_COMMAND_CreateDebugMesh);
}
bool OSMElementComponent::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool OSMElementComponent::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_Way], &m_Way  );
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_DebugWayModel], &m_DebugWayModel  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_COMMAND_CreateDebugMesh], &m_COMMAND_CreateDebugMesh  );
	return true;
}
void OSMElementComponent::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	OSMElementComponent* other = (OSMElementComponent*) otherObj;
	other->SetWay(m_Way);
	other->SetDebugWayModel(m_DebugWayModel);
	other->SetCOMMAND_CreateDebugMesh(m_COMMAND_CreateDebugMesh);
}
const char* OSMElementComponent::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE OSMElementComponent


void OSMElementComponent::Update()
{

}


void OSMElementComponent::DebugDisplay()
{


}

void OSMElementComponent::SetCOMMAND_CreateDebugMesh_Callback(const bool& value)
{
	if (value) 
	{
		std::vector<glm::vec3> flatWayPositions;
		//todo : make it work with an index buffer someday
		std::vector<WayData> vertexData;

		Way* way = m_Way;

		std::vector<Node*>& nodes = way->GetNodes();

		// loop over all nodes and find the min/max node.
		lx2I64 minPos = lx2I64(LX_LIMITS_INT64_MAX, LX_LIMITS_INT64_MAX);
		lx2I64 maxPos = lx2I64(LX_LIMITS_INT64_MIN, LX_LIMITS_INT64_MIN);
		for (int i = 0; i < nodes.size(); ++i)
		{
			Node* node = nodes[i];

			const lx2I64& nodePos = node->GetHighPrecisionEarthCoordinates();
			minPos = glm::min(minPos, nodePos);
			maxPos = glm::max(maxPos, nodePos);
		}

		lx2I64 max64 = lx2I64(LX_LIMITS_INT64_MAX, LX_LIMITS_INT64_MAX);
		lx2I64 halfMax64 = max64 / lx2I64(2, 2);
		lxAssert(glm::all(glm::lessThan(minPos, halfMax64)));
		lxAssert(glm::all(glm::lessThan(maxPos, halfMax64)));

		lx2I64 centroid = (maxPos + minPos) / lx2I64(2, 2);

		// size from min/max to centroid
		lx2I64 bbScale = maxPos - centroid;
		lx2F32 bbSizeF = glm::vec2(bbScale);

		for (int i = 0; i < nodes.size(); ++i)
		{
			Node* node = nodes[i];

			if (i > 1)
			{
				lx3F32 lastNodelModelPosF3 = flatWayPositions[flatWayPositions.size() - 1];
				flatWayPositions.push_back(lastNodelModelPosF3);
				vertexData.push_back({ way->GetOSMElementType(), way->GetIndexInSector() });
			}

			lx3F32 nodelModelPosF3 = node->GetWorldPosition();
			flatWayPositions.push_back(nodelModelPosF3);
			vertexData.push_back({ way->GetOSMElementType(), way->GetIndexInSector() });
		}

		Model* flatWaysModel = RenderDataManager::CreateFlatDebugModel(flatWayPositions, vertexData, glm::vec3(1, 1, 1), "Sector_FlatLines_");
		m_DebugWayModel = flatWaysModel;
	}

	m_COMMAND_CreateDebugMesh = false;
}
