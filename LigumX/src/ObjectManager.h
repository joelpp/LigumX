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

// todo jpp : likely a huge waste but eh
struct FileDisplayInformation
{
	ObjectID m_ObjectID;
	LXString m_Typename;
	LXString m_Name;
	LXString m_AsText;

	FileDisplayInformation(const LXString& fileName);
};

class ObjectManager
{
public:
	ObjectManager();

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

	ObjectPtr FindObjectByID(ObjectID id, bool createIfNotFound);
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
		return (T*) (m_ObjectMap[objectID]);
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

		auto it = m_ObjectMap.find(id);

		IncrementObjectMapHits();

		if (it == m_ObjectMap.end())
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

	std::vector<FileDisplayInformation>& GetAllFiles(bool forceUpdate=false);
	void UpdateFileList();

	LXObject* GetObjectFromIDAndType(bool createIfNotLoaded, ObjectID objectID, const std::string& typeName);
	LXObject* CreateObject(const std::string& typeName, ObjectID id);
	LXObject* CreateNewObject(const std::string& typeName);

	LXType GetLXTypeFromClassname(const std::string& Classname);
	std::string GetClassnameFromLXType(const LXType type);

private:
	int m_NextTransientID = StartTransientIDs;
	int m_NextHardcodedID;

	// replace with shared ptr eventually
	ObjectMap m_ObjectMap;
	std::vector<LXType> m_SupportedTypes;

	std::vector<FileDisplayInformation> m_AllFiles;

	ObjectID m_MaxID = 0;

public:
	LXType g_ParentClass[LXType_Count];
	std::vector<LXType> g_ChildClasses[LXType_Count];

};

extern ObjectManager* g_ObjectManager;
