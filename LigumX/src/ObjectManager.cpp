#include "ObjectManager.h"
#include <cstddef>
#include <random>
#include "EngineStats.h"
#include "stringutils.h"
#include "fileutils.h"
#include "ObjectFactory.h"

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
	} while ((unsigned int) r >= limit);

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


	// todo jpp is this REALLY needed? seems a bit intense
	m_SupportedTypes.push_back(LXType_Texture);
	m_SupportedTypes.push_back(LXType_Mesh);
	m_SupportedTypes.push_back(LXType_Material);
	m_SupportedTypes.push_back(LXType_Model);
	m_SupportedTypes.push_back(LXType_Entity);
	m_SupportedTypes.push_back(LXType_World);
	m_SupportedTypes.push_back(LXType_Visual);
	m_SupportedTypes.push_back(LXType_SunLight);

	Initialize();
}


void ObjectManager::Initialize()
{
#define DETECT_ID_DUPLICATES 1

#if DETECT_ID_DUPLICATES 
	std::vector<int> dupes;
#endif //DETECT_ID_DUPLICATES 

	std::vector<FileDisplayInformation>& allFiles = GetAllFiles();
	int maxID = 0;

	for (FileDisplayInformation& str : allFiles)
	{
		ObjectID id = str.m_ObjectID;

#if DETECT_ID_DUPLICATES 
		for (int i = 0; i < dupes.size(); ++i)
		{
			if (dupes[i] == id)
			{
				lxAssert0();
			}
		}
#endif

		maxID = std::max(id, maxID);
#if DETECT_ID_DUPLICATES 
		dupes.push_back(id);
#endif

		m_MaxID = maxID;
	}

#define LX_CLASS(c, p) g_ParentClass[LXType_##c] = LXType_##p;
#define LX_ENUM(c)
#include "LXClassList.h"
#undef LX_CLASS
#undef LX_ENUM

#define LX_CLASS(c, p) g_ChildClasses[LXType_##p].push_back(LXType_##c);
#define LX_ENUM(c)
#include "LXClassList.h"
#undef LX_CLASS
#undef LX_ENUM

}


int ObjectManager::GetNewObjectID()
{
	return m_MaxID++;
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

ObjectPtr ObjectManager::FindObjectByID(ObjectID id, bool createIfNotFound)
{
	auto it = m_ObjectMap.find(id);

	g_EngineStats->AddTo_NumObjectMapHits(1);

	if (it == m_ObjectMap.end())
	{
		if (createIfNotFound)
		{
			// todo: we need to allocate a something*. not sure about the best way to do so now.
			lxAssert0();
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

	m_ObjectMap[id] = ptr;

	return true;
}

bool ObjectManager::FilenameIsID(std::string& s)
{
	return StringUtils::StringContains(s, '<');
}

void ObjectManager::IncrementObjectMapHits()
{
	g_EngineStats->AddTo_NumObjectMapHits(1);
}

std::vector<FileDisplayInformation>& ObjectManager::GetAllFiles(bool forceUpdate)
{
	if (m_AllFiles.size() == 0 || forceUpdate)
	{
		m_AllFiles.clear();
		StringList allFileNames = FileUtils::GetAllFilesInDirectory(g_PathObjects.c_str());

		for (LXString& fileName : allFileNames)
		{
			m_AllFiles.push_back(FileDisplayInformation(fileName));
		}
	}

	return m_AllFiles;
}

void ObjectManager::UpdateFileList()
{
	GetAllFiles(true);
}

LXObject* ObjectManager::CreateObject(const std::string& typeName, ObjectID id)
{
	int classHash = std::hash_value(typeName);

	LXObject* newObject = ObjectFactory::GetNewObject(classHash, id);// Visual::GetNewChildObject();
	return newObject;
}

LXObject* ObjectManager::CreateNewObject(const std::string& typeName)
{
	ObjectID newID = GetNewObjectID();
	return CreateObject(typeName, newID);
}

LXObject* ObjectManager::CreateNewObject(LXType lxType)
{
	return CreateNewObject(GetClassnameFromLXType(lxType));
}

LXObject* ObjectManager::GetObjectFromIDAndType(bool createIfNotLoaded, ObjectID objectID, const std::string& typeName)
{
	ObjectPtr existingObject = FindObjectByID(objectID, false);
			
	if (existingObject != nullptr)
	{
		return (LXObject*)existingObject;
	}
	else if (createIfNotLoaded)
	{
		int classHash = std::hash_value(typeName);

		LXObject* newObject = ObjectFactory::GetNewObject(classHash, objectID);// Visual::GetNewChildObject();
		return newObject;
	}

	return nullptr;
}

LXObject* ObjectManager::LoadRawObjectFromFile(ObjectID objectID, const std::string& typeName)
{
	int classHash = std::hash_value(typeName);

	bool trackInObjectManager = false;
	LXObject* newObject = ObjectFactory::GetNewObject(classHash, objectID, trackInObjectManager);// Visual::GetNewChildObject();
	return newObject;
}

FileDisplayInformation::FileDisplayInformation(const LXString& fileName)
{
	bool valid = false;
	std::vector<LXString> all = StringUtils::SplitString(fileName, '_');

	if (all.size() == 2)
	{
		std::vector<LXString> idType = StringUtils::SplitString(all[1], '.');

		if (idType.size() == 2)
		{
			m_ObjectID = StringUtils::ToInt(idType[0]);
			m_Typename = all[0];
			valid = true;
		}
	}

	lxAssert(valid);
	if (valid)
	{
		// go get the name
		// todo jpp : refactor this into a kind of "get data value from file"
		std::fstream file = FileUtils::OpenFile((g_PathObjects + fileName).c_str());

		if (file.is_open())
		{
			std::string line;
			bool foundName = false;
			while (std::getline(file, line))
			{
				if (line == "Name" && !foundName)
				{
					foundName = true;
					continue;
				}
				if (foundName)
				{
					m_Name = line;
					break;
				}
			}
			file.close();
		}
		m_AsText = m_Typename + " (" + std::to_string(m_ObjectID) + ") : " + m_Name;
	}

}

LXType ObjectManager::GetLXTypeFromClassname(const std::string& className)
{
	#define LX_CLASS(c, p) if (className == "##c##") { return LXType_##c##; }
	#define LX_ENUM(c)
	#include "LXClassList.h"
	#undef LX_CLASS
	#undef LX_ENUM

	return LXType_None;
}


std::string ObjectManager::GetClassnameFromLXType(const LXType type)
{
#define LX_CLASS(c, p) if (type == LXType_##c##) { return #c; }
#define LX_ENUM(c) if (type == LXType_##c##) { return "!!FIX THIS!! enum dont use "; }
#include "LXClassList.h"
#undef LX_CLASS
#undef LX_ENUM

	if (type == LXType_stdvector) { return std::string("std::vector"); }
	if (type == LXType_int) { return std::string("int"); }
	if (type == LXType_bool) { return std::string("bool"); }
	if (type == LXType_float) { return std::string("float"); }
	if (type == LXType_glmvec2) { return std::string("glm::vec2"); }
	if (type == LXType_glmvec3) { return std::string("glm::vec3"); }
	if (type == LXType_glmvec4) { return std::string("glm::vec4"); }
	if (type == LXType_glmvec4) { return std::string("glm::vec4"); }

	lxAssert0();
	return "error!";
}

LXObject* ObjectManager::CloneObject(LXObject* objToClone)
{
	LXObject* newObject = g_ObjectManager->CreateNewObject(objToClone->GetLXType());
	ObjectID newObjectID = newObject->GetObjectID();
	objToClone->Clone(newObject);

	std::string newName = objToClone->GetName() + " COPY";
	newObject->SetName(newName);
	newObject->SetObjectID(newObjectID);

	return newObject;
}

void ObjectManager::ReloadObject(LXObject** objToReload)
{
	LXObject* tempObjFromFile = LoadRawObjectFromFile((*objToReload)->GetObjectID(), (*objToReload)->GetLXClassName());
	tempObjFromFile->Clone(*objToReload);
	delete(tempObjFromFile);
}
