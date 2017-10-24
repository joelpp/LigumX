#pragma once

#include <map>

typedef int ObjectID;
typedef const char* ObjectPtr;

class ObjectIDManager
{
public:
	ObjectIDManager();

	ObjectID GetNewObjectID();
	ObjectID GetTransientID();

	bool IsValidID(ObjectID id) { return id == InvalidID; }
	bool IsHardcodedID(ObjectID id) { return id >= StartHardcodedIDs && id < StartTransientIDs;  }

	const int InvalidID = 0;
	const int StartHardcodedIDs = 1;
	const int StartTransientIDs = 2048;
	const int StartObjectIDs = 8192;

	ObjectID DefaultSphereMeshID;
	ObjectID DefaultQuadMeshID;
	ObjectID DefaultCubeMeshID;

	ObjectPtr FindObjectByID(ObjectID id, bool createIfNotFound);
	void AddObject(ObjectID id, ObjectPtr ptr);

private:
	int m_NextTransientID = StartTransientIDs;
	int m_NextHardcodedID;

	// replace with shared ptr eventually
	std::map<ObjectID, ObjectPtr> m_Objects;
};

extern ObjectIDManager* g_ObjectIDManager;
