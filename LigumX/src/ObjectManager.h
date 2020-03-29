#pragma once

#include <map>
#include <vector>
#include "property.h"
#include "LXTypeDefinition.h"

class LXObject;
typedef int ObjectID;
typedef char* ObjectPtr;
typedef std::map<ObjectID, ObjectPtr> ObjectMap;

const std::string g_PathMainData	= "C:/Users/Joel/Documents/LigumX/LigumX/data/";
const std::string g_PathModels		= g_PathMainData + "models/";
const std::string g_PathTextures	= g_PathMainData + "textures/";
const std::string g_PathObjects		= g_PathMainData + "objects/";
const std::string g_PathObjectsBkp  = g_PathMainData + "objects_backup/";
const std::string g_PathShaders		= g_PathMainData + "shaders/";

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
	ObjectID DefaultTerrainMeshID;
	ObjectID DefaultLineMeshID;

	ObjectID DefaultManipulatorEntityID;
	ObjectID DefaultCubeModelID;
	ObjectID DefaultRedMaterialID;

	ObjectPtr FindObjectByID(ObjectID id, LXType type, bool createIfNotFound);
	bool AddObject(ObjectID id, LXType type, ObjectPtr ptr);
	
	void IncrementObjectMapHits();

	template <typename T>
	bool AddObject(LXType type, T* ptr)
	{
		return AddObject(ptr->GetObjectID(), type, (ObjectPtr)ptr);
	}

	template <typename T>
	bool AddObject(T* ptr)
	{
		return AddObject(T::Type, ptr);
	}

	template <typename T>
	T* GetLXObject(int objectID)
	{
		return (T*) (m_ObjectMaps[T::Type][objectID]);
	}

	template <typename T>
	T* LoadObject(ObjectID id)
	{
		T* newObject = new T();
		newObject->SetObjectID(id);
		newObject->Serialize(false);

		return newObject;
	}


	template <typename T>
	T* FindObjectByID(ObjectID id, bool createIfNotFound = false)
	{
		LXType type = T::Type;

		if (!IsSupportedType(type))
		{
			return nullptr;
		}

		auto it = m_ObjectMaps[type].find(id);

		IncrementObjectMapHits();

		if (it == m_ObjectMaps[type].end())
		{
			if (createIfNotFound)
			{
				T* newObject = LoadObject<T>(id);
				AddObject<T>(newObject);
				return newObject;
			}
			else
			{
				return nullptr;
			}
		}
		else
		{
			return (T*) it->second;
		}
	}

	template <typename T>
	T* CreateObject()
	{
		ObjectID id = GetTransientID();

		T* newObject = new T();
		newObject->SetObjectID(id);

		return newObject;
	}

	std::vector<LXString>& GetAllFiles();

	

private:
	int m_NextTransientID = StartTransientIDs;
	int m_NextHardcodedID;

	// replace with shared ptr eventually
	std::map<LXType, ObjectMap> m_ObjectMaps;
	std::vector<LXType> m_SupportedTypes;

	std::vector<LXString> m_AllFiles;

	ObjectID m_MaxID = 0;

	LXObject* GetObjectFromFilename(std::string& str);

};

extern ObjectManager* g_ObjectManager;
