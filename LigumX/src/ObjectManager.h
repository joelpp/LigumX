#pragma once

#include <map>
#include <vector>
#include "property.h"

typedef int ObjectID;
typedef char* ObjectPtr;
typedef std::map<ObjectID, ObjectPtr> ObjectMap;

class ObjectManager
{
public:
	ObjectManager();

	std::map<ObjectID, ObjectPtr>* GetObjects(LXType type) { return &(m_ObjectMaps[type]); };
	bool IsSupportedType(LXType type);
	void Initialize();

	ObjectID GetNewObjectID();
	ObjectID GetTransientID();

	bool IsValidID(ObjectID id) { return id == InvalidID; }
	bool IsHardcodedID(ObjectID id) { return id >= StartHardcodedIDs && id < StartTransientIDs;  }
	bool FilenameIsID(std::string& s);

	const int InvalidID = 0;
	const int StartHardcodedIDs = 1;
	const int StartTransientIDs = 2048;
	const int StartObjectIDs = 8192;

	ObjectID DefaultSphereMeshID;
	ObjectID DefaultQuadMeshID;
	ObjectID DefaultCubeMeshID;

	ObjectPtr FindObjectByID(ObjectID id, LXType type, bool createIfNotFound);
	bool AddObject(ObjectID id, LXType type, ObjectPtr ptr);

	template <typename T>
	bool AddObject(LXType type, T* ptr)
	{
		return AddObject(ptr->GetObjectID(), type, (ObjectPtr)ptr);
	}

private:
	int m_NextTransientID = StartTransientIDs;
	int m_NextHardcodedID;

	// replace with shared ptr eventually
	std::map<LXType, ObjectMap> m_ObjectMaps;
	std::vector<LXType> m_SupportedTypes;
};

extern ObjectManager* g_ObjectManager;
