#include "ObjectManager.h"
#include <cstddef>
#include <random>
#include "EngineStats.h"
#include "stringutils.h"

ObjectManager* g_ObjectManager;

unsigned int rand_interval(unsigned int min, unsigned int max)
{
	int r;
	const unsigned int range = 1 + max - min;
	const unsigned int buckets = RAND_MAX / range;
	const unsigned int limit = buckets * range;

	/* Create equal size buckets all in a row, then fire randomly towards
	* the buckets until you land in one of them. All buckets are equally
	* likely. If you land off the end of the line of buckets, try again. */
	do
	{
		r = rand();
	} while (r >= limit);

	return min + (r / buckets);
}

ObjectManager::ObjectManager()
{
	m_NextHardcodedID = StartHardcodedIDs;

	DefaultSphereMeshID			= m_NextHardcodedID++;
	DefaultQuadMeshID			= m_NextHardcodedID++;
	DefaultCubeMeshID			= m_NextHardcodedID++;
	DefaultTerrainMeshID		= m_NextHardcodedID++;
	DefaultLineMeshID			= m_NextHardcodedID++;

	DefaultRedMaterialID		= m_NextHardcodedID++;

	DefaultCubeModelID			= m_NextHardcodedID++;
	DefaultManipulatorEntityID	= m_NextHardcodedID++;

	m_SupportedTypes.push_back(LXType_Texture);
	m_SupportedTypes.push_back(LXType_Mesh);
	m_SupportedTypes.push_back(LXType_Material);
	m_SupportedTypes.push_back(LXType_Model);

	Initialize();
}

void ObjectManager::Initialize()
{
	for (LXType& type : m_SupportedTypes)
	{
		m_ObjectMaps[type] = ObjectMap();
	}
}


int ObjectManager::GetNewObjectID()
{
	return rand_interval(StartObjectIDs, RAND_MAX);
}


int ObjectManager::GetTransientID()
{
	int toReturn = m_NextTransientID;
	m_NextTransientID++;
	return toReturn;
}

bool ObjectManager::IsSupportedType(LXType type)
{
	for (LXType& atype : m_SupportedTypes)
	{
		if (type == atype)
		{
			return true;
		}
	}

	return false;
}

ObjectPtr ObjectManager::FindObjectByID(ObjectID id, LXType type, bool createIfNotFound)
{
	if (!IsSupportedType(type))
	{
		return nullptr;
	}

	auto it = m_ObjectMaps[type].find(id);

	g_EngineStats->AddToNumObjectMapHits(1);

	if (it == m_ObjectMaps[type].end())
	{
		if (createIfNotFound)
		{
			// todo: we need to allocate a something*. not sure about the best way to do so now.
			return nullptr;
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		return it->second;
	}
}

bool ObjectManager::AddObject(ObjectID id, LXType type, ObjectPtr ptr)
{
	if (!IsSupportedType(type))
	{
		return false;
	}

	m_ObjectMaps[type][id] = ptr;

	return true;
}

bool ObjectManager::FilenameIsID(std::string& s)
{
	return StringUtils::StringContains(s, '<');
}
