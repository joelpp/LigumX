#include "ObjectIDManager.h"
#include <cstddef>
#include <random>

ObjectIDManager* g_ObjectIDManager;

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

ObjectIDManager::ObjectIDManager()
{
	m_NextHardcodedID = StartHardcodedIDs;
	DefaultSphereMeshID = m_NextHardcodedID++;
	DefaultQuadMeshID = m_NextHardcodedID++;
	DefaultCubeMeshID = m_NextHardcodedID++;
}

int ObjectIDManager::GetNewObjectID()
{
	return rand_interval(StartObjectIDs, RAND_MAX);
}


int ObjectIDManager::GetTransientID()
{
	int toReturn = m_NextTransientID;
	m_NextTransientID++;
	return toReturn;
}

ObjectPtr ObjectIDManager::FindObjectByID(ObjectID id, bool createIfNotFound)
{
	auto it = m_Objects.find(id);

	if (it == m_Objects.end())
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

void ObjectIDManager::AddObject(ObjectID id, ObjectPtr ptr)
{
	m_Objects[id] = ptr;
}