#include "ObjectIDManager.h"

ObjectIDManager* g_ObjectIDManager;

ObjectIDManager::ObjectIDManager()
{
	m_NextHardcodedID = StartHardcodedIDs;
	DefaultSphereMeshID = m_NextHardcodedID++;
	DefaultQuadMeshID = m_NextHardcodedID++;
	DefaultCubeMeshID = m_NextHardcodedID++;
}

int ObjectIDManager::GetTransientID()
{
	int toReturn = m_NextTransientID;
	m_NextTransientID++;
	return toReturn;
}
