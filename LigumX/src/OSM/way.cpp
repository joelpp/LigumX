
#include "way.h"
#include "node.h"
#include "OSMElement.h"
#include "linesegment.h"

#pragma region  CLASS_SOURCE Way

#include "Way.h"
#include "serializer.h"
#include "Node.h"
const ClassPropertyData Way::g_Properties[] = 
{
{ "OSMId", PIDX_OSMId, offsetof(Way, m_OSMId), 0, LXType_Object, sizeof(lxInt64), LXType_lxInt64, false, LXType_None, false, 0, LX_LIMITS_INT64_MIN, LX_LIMITS_INT64_MAX, 0,}, 
{ "Nodes", PIDX_Nodes, offsetof(Way, m_Nodes), 0, LXType_stdvector, sizeof(std::vector<Node*>), LXType_stdvector, false, LXType_Node, true, 0, 0, 0, 0,}, 
{ "OSMElementType", PIDX_OSMElementType, offsetof(Way, m_OSMElementType), 0, LXType_Object, sizeof(OSMElementType), LXType_OSMElementType, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "IndexInSector", PIDX_IndexInSector, offsetof(Way, m_IndexInSector), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "FilledIn", PIDX_FilledIn, offsetof(Way, m_FilledIn), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "AllTags", PIDX_AllTags, offsetof(Way, m_AllTags), 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, false, PropertyFlags_Adder, 0, 0, 0,}, 
};
void Way::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeInt64(g_Properties[PIDX_OSMId], m_OSMId);
	serializer.SerializeVector(g_Properties[PIDX_Nodes], m_Nodes);
	serializer.SerializeInt(g_Properties[PIDX_IndexInSector], m_IndexInSector);
	serializer.SerializeBool(g_Properties[PIDX_FilledIn], m_FilledIn);
	serializer.SerializeString(g_Properties[PIDX_AllTags], m_AllTags);
}
bool Way::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool Way::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_OSMId], &m_OSMId , LX_LIMITS_INT64_MIN, LX_LIMITS_INT64_MAX );
	ImguiHelpers::ShowProperty3(this, g_Properties[PIDX_Nodes], m_Nodes  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_IndexInSector], &m_IndexInSector , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_FilledIn], &m_FilledIn  );
	ImguiHelpers::ShowString2(this, g_Properties[PIDX_AllTags], m_AllTags  );
	return true;
}
void Way::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	Way* other = (Way*) otherObj;
	other->SetOSMId(m_OSMId);
	other->SetNodes(m_Nodes);
	other->SetOSMElementType(m_OSMElementType);
	other->SetIndexInSector(m_IndexInSector);
	other->SetFilledIn(m_FilledIn);
	other->SetAllTags(m_AllTags);
}
const char* Way::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE Way
Way::Way()
{

}

Way::Way(const std::string& id)
{
    this->id = id;
}

Way::Way(std::string _id, LineSegment ls, int type)
{
    this->id = _id;

    AddNode(new Node("n0", ls.p0.x, ls.p0.y));
	AddNode(new Node("n1", ls.p1.x, ls.p1.y));
}

void Way::AddNode(Node* ref)
{
    m_Nodes.push_back(ref);
}

std::string Way::toString()
{
    char str[200];
    sprintf(str, "Way ID=%s, %I64u nodes, eType: %s, ", this->id.c_str(), m_Nodes.size(), EnumValues_OSMElementType[this->GetOSMElementType()].c_str());
    std::string toReturn = std::string(str);
    for ( auto it = this->tags.begin(); it != this->tags.end(); ++it ){
        char tagstr[200];
        sprintf(tagstr, " | k: %s v: %s |", it->first.c_str(), it->second.c_str());
        toReturn.append(std::string(tagstr));
    }
    return toReturn;
}
bool Way::hasPointInside(glm::vec2 xy)
{

    //If this way doesn't form a closed loop the concept of "being inside" it is meaningless. MEANINGLESS!!
//    if(!loops()) return false;
//    std::cout << "forms loop";
    bool first = true;
    int numIntersections = 0;
    LineSegment L0;
    L0.p0 = xy;
    L0.p1 = glm::vec2(2,2); //def not future proof. or is it?
    LineSegment L1;
    Node* node1 = NULL;
    Node* node2 = NULL;

    // Loop over all this way's nodes
    for (auto it = this->m_Nodes.begin(); it != this->m_Nodes.end(); ++it){
        // On first pass set first node and carry on
        if (first){
            node1 = *it;
            first = false;
            continue;
        }

        // Define a linesegment between the two current nodes
        node2 = *it;
        L1.p0 = node1->getLatLong();
        L1.p1 = node2->getLatLong();

        // Add to the counter if L0 crossed the way's perimeter
        if (L0.intersects(L1, NULL, NULL)){
//            std::cout << L0.toString() << "\n";
//            std::cout << L1.toString() << "\n";
            numIntersections++;
        }
        //Set first node to previous second node
        node1 = node2;
    }
    // Odd number of intersections means the point was inside the perimeter
    return ((numIntersections % 2) == 1);
}

bool Way::loops()
{
    auto it = this->m_Nodes.begin();
    auto it2 = this->m_Nodes.end();

    return (it == it2);
}



WayPair Way::findCommon(std::vector<Way*> firstNodeWays, std::vector<Way*> secondNodeWays)
{
    for (int i = 0; i < firstNodeWays.size(); i++)
	{
        for (int j = 0; j < secondNodeWays.size(); j++)
		{
            if (firstNodeWays[i]->id.compare(secondNodeWays[j]->id) == 0) return std::pair<int, int>(i,j);
        }
    }

    return WayPair(-1,-1);
}
