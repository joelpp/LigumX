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

	std::vector<LXString>& allFiles = GetAllFiles();
	int maxID = 0;

	for (LXString& str : allFiles)
	{
		std::vector<LXString> all = StringUtils::SplitString(str, '_');

		if (all.size() == 2)
		{
			std::vector<LXString> idType = StringUtils::SplitString(all[1], '.');

			if (idType.size() == 2)
			{
				ObjectID id = StringUtils::ToInt(idType[0]);

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

			}
		}

		m_MaxID = maxID;
	}

	


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

std::vector<LXString>& ObjectManager::GetAllFiles(bool forceUpdate)
{
	if (m_AllFiles.size() == 0 || forceUpdate)
	{
		m_AllFiles.clear();
		m_AllFiles = FileUtils::GetAllFilesInDirectory(g_PathObjects.c_str());
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

LXObject* ObjectManager::GetObjectFromFilename(bool createIfNotLoaded, const std::string& str)
{
	std::vector<LXString> all = StringUtils::SplitString(str, '_');

	if (all.size() == 2)
	{
		std::vector<LXString> idType = StringUtils::SplitString(all[1], '.');

		if (idType.size() == 2)
		{
			ObjectID id = StringUtils::ToInt(idType[0]);

			ObjectPtr existingObject = FindObjectByID(id, false);
			
			if (existingObject != nullptr)
			{
				return (LXObject*)existingObject;
			}
			else if (createIfNotLoaded)
			{
				std::string& typeName = all[0];
				int classHash = std::hash_value(typeName);

				LXObject* newObject = ObjectFactory::GetNewObject(classHash, id);// Visual::GetNewChildObject();
				return newObject;
			}
		}
	}

	return nullptr;
}
