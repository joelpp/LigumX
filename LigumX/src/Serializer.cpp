#include "serializer.h"
#include "DisplayOptions.h"
#include "EditorOptions.h"
#include "PostEffects.h"
#include "Camera.h"
#include "Model.h"
#include "DefaultObjects.h"
#include "Material.h"
#include "Component.h"
#include "Entity.h"
#include "Texture.h"
#include "Editor.h"
#include "Sunlight.h"
#include "Mesh.h"
#include "GL.h"
#include "StringUtils.h"
#include "glm/glm.hpp"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem> 

Serializer* g_Serializer;

Serializer::Serializer()
{
}

#define SERIALIZE_PTR_OUT(type) \
case LXType_##type : \
{ \
	##type* val = (##type*) ptr; \
	if (ptr) \
	{ \
		objectStream << val->GetObjectID() << std::endl; \
		val->Serialize(true); \
	} \
	else \
	{ \
		objectStream << 0 << std::endl; \
	} \
	break; \
} \

#define SERIALIZE_VALUE_OUT(type, name) \
case LXType_##type : \
{ \
objectStream << *((##type *)ptr) << std::endl; \
break; \
} \

#define SERIALIZE_VALUE_OUT_NS(nameSpace, type, name) \
case LXType_##nameSpace##type : \
{ \
objectStream << *((##nameSpace::##type *)ptr) << std::endl; \
break; \
} \


void Serializer::SerializePropertyOut(const char* ptr, const char* name, const LXType& type, const LXType& simpleType, std::fstream& objectStream)
{
	switch (type)
	{
		SERIALIZE_VALUE_OUT(int, name)
		SERIALIZE_VALUE_OUT(bool, name)
		SERIALIZE_VALUE_OUT(float, name)
		SERIALIZE_VALUE_OUT_NS(std, string, name)

		case LXType_glmvec3:
		{
			glm::vec3* value = (glm::vec3*) ptr;
			objectStream << value->x << std::endl;
			objectStream << value->y << std::endl;
			objectStream << value->z << std::endl;
			break;
		}
		case LXType_glmvec2:
		{
			glm::vec2* value = (glm::vec2*) ptr;
			objectStream << value->x << std::endl;
			objectStream << value->y << std::endl;
			break;
		}
		case LXType_glmivec2:
		{
			glm::ivec2* value = (glm::ivec2*) ptr;
			objectStream << value->x << std::endl;
			objectStream << value->y << std::endl;
			break;
		}
		SERIALIZE_PTR_OUT(DisplayOptions)
		SERIALIZE_PTR_OUT(EditorOptions)
		SERIALIZE_PTR_OUT(PostEffects)
		SERIALIZE_PTR_OUT(Camera)
		SERIALIZE_PTR_OUT(Texture)
		SERIALIZE_PTR_OUT(SunLight)
		SERIALIZE_PTR_OUT(Entity)
		SERIALIZE_PTR_OUT(Model)
		//SERIALIZE_PTR_OUT(Mesh)
		SERIALIZE_PTR_OUT(Material)
		SERIALIZE_PTR_OUT(Editor)


		case LXType_Mesh:
		{
			Mesh* val = (Mesh*) ptr;
			if (ptr)
			{
				int id = val->GetObjectID();
				objectStream << id << std::endl;

				if (g_ObjectManager->IsHardcodedID(id))
				{

				}
				else
				{
					val->Serialize(true);
				}
			}
			else
			{
				objectStream << 0 << std::endl;
			}
			break;
		}

		//case LXType_stdstring:
		//{
		//ShowProperty((std::string*) ptr, name);
		//break;
		//}
		//case LXType_Model:
		//{
		//	objectStream << name << std::endl;

		//	Model* model = (Model*) ptr;
		//	objectStream << model->GetObjectID() << std::endl;

		//	model->Serialize(true);
		//	break;
		//}
		//case LXType_Material:
		//{
		//	Material* material = (Material*)ptr;

		//	objectStream << material->GetObjectID() << std::endl;

		//	material->Serialize(true);
		//	break;
		//}

		//case LXType_Entity:
		//{
		//	Entity* entity = (Entity*)ptr;

		//	objectStream << entity->GetObjectID() << std::endl;

		//	entity->Serialize(true);
		//	break;
		//}

		//case LXType_Texture:
		//{
		//if (ptr)
		//{
		//ShowGUIText(name);
		//ImGui::Image((ImTextureID)((Texture*)ptr)->GetHWObject(), ImVec2(50, 50));
		//}

		//break;
		//}
		case LXType_Component:
		{
			if (ptr)
			{
				Component* component = (Component*)ptr;
				//objectStream << component->GetObjectID() << std::endl;

				component->Serialize(true);
				break;
			}

			break;
		}
		//case LXType_AABB:
		//{
		//if (ptr)
		//{
		//ShowPropertyGridTemplate<AABB>((AABB*)ptr, name);
		//}

		//break;
		//}
		default:
		{
			break;
		}
	}
}

#define SERIALIZE_PRIMITIVE_IN(type) \
case LXType_##type : \
{ \
objectStream >> *((##type *)ptr); \
break; \
} \

#define SERIALIZE_PTR_IN(className) \
case LXType_##className : \
{ \
	int objectID; \
	objectStream >> objectID; \
	if (objectID == 0) \
	{ \
		break; \
	} \
	\
	bool mustSerialize = false; \
	char* loadedObject = (char*) g_ObjectManager->FindObjectByID<className>(objectID); \
	\
	##className* dptr = nullptr; \
	if (loadedObject == nullptr) \
	{ \
		if (*ptr == 0) \
		{ \
			dptr = new className(); \
			g_ObjectManager->AddObject(objectID, type, (ObjectPtr) dptr); \
		} \
		mustSerialize = true; \
	} \
	else \
	{ \
		dptr = (##className *)loadedObject; \
	} \
	char** aptr = (char**)ptr; \
	*aptr = (char*)dptr; \
	ptr = *(char**)ptr; \
	if (mustSerialize) \
	{ \
		##className* val = (##className *)ptr; \
		val->SetObjectID(objectID); \
		val->Serialize(false); \
	} \
	break; \
} \

#define SERIALIZE_PRIMITIVE_IN_NS(nameSpace, type) \
case LXType_ ##nameSpace##type : \
{ \
objectStream >> *((##nameSpace::##type *)ptr); \
break; \
} \

void Serializer::SerializePropertyIn(char*& ptr, const LXType& type, const LXType& associatedType, const LXType& simpleType, std::fstream& objectStream)
{
	switch (type)
	{
		SERIALIZE_PRIMITIVE_IN(int)
		SERIALIZE_PRIMITIVE_IN(bool)
		SERIALIZE_PRIMITIVE_IN(float)
		//SERIALIZE_PRIMITIVE_IN_NS(std, string)

		case LXType_stdstring:
		{
			std::string* sptr = (std::string *)ptr;
			std::string temp;
			objectStream >> temp;
			*sptr = std::string(temp.c_str());
			break;
		} 

		case LXType_glmvec3:
		{
			glm::vec3* vector = (glm::vec3*) ptr;

			for (int i = 0; i < 3; ++i)
			{
				objectStream >> (*vector)[i];
			}

			break;
		}
		case LXType_glmvec2:
		{
			glm::vec2* vector = (glm::vec2*) ptr;
			for (int i = 0; i < 2; ++i)
			{
				objectStream >> (*vector)[i];
			}

			break;
		}

		case LXType_glmivec2:
		{
			glm::ivec2* vector = (glm::ivec2*) ptr;
			for (int i = 0; i < 2; ++i)
			{
				objectStream >> (*vector)[i];
			}
			break;
		}

		SERIALIZE_PTR_IN(DisplayOptions)


	//case LXType_DisplayOptions :
	//{
	//	int objectID;
	//	objectStream >> objectID;
	//	if (objectID == 0)
	//	{
	//		break;
	//	} 
	//	
	//	bool mustSerialize = false; 
	//	ObjectPtr loadedObject = g_ObjectManager->FindObjectByID(objectID, type, false); 
	//
	//	DisplayOptions* dptr = nullptr; 
	//	if (loadedObject == nullptr)
	//	{
	//		if (*ptr == 0)
	//		{
	//			dptr = new DisplayOptions();
	//			g_ObjectManager->AddObject(objectID, type, (ObjectPtr) dptr);
	//		}
	//		mustSerialize = true;
	//	}
	//	else
	//	{
	//		dptr = (DisplayOptions *)loadedObject;
	//	}
	//	char** aptr = (char**)ptr;
	//	*aptr = (char*)dptr;
	//	ptr = *(char**)ptr;
	//	if (mustSerialize)
	//	{
	//		DisplayOptions* val = (DisplayOptions *)ptr;
	//		val->SetObjectID(objectID);
	//		val->Serialize(false);
	//	}
	//	break;
	//}



		SERIALIZE_PTR_IN(PostEffects)
		SERIALIZE_PTR_IN(Camera)
		SERIALIZE_PTR_IN(Material)
		SERIALIZE_PTR_IN(Model)
		//SERIALIZE_PTR_IN(Mesh)
		//SERIALIZE_PTR_IN(Texture)
		SERIALIZE_PTR_IN(SunLight)
		SERIALIZE_PTR_IN(Entity)
		SERIALIZE_PTR_IN(Editor)
		SERIALIZE_PTR_IN(EditorOptions)
		SERIALIZE_PTR_IN(Texture)

		//case LXType_Texture:
		//{
		//	int objectID;
		//	objectStream >> objectID;
		//	if (objectID == 0)
		//	{
		//		break;
		//	}

		//	bool mustSerialize = false;

		//	ObjectPtr loadedObject = g_ObjectManager->FindObjectByID(objectID, type, false);

		//	Texture* dptr = nullptr;
		//	if (loadedObject == nullptr)
		//	{
		//		if (*ptr == 0)
		//		{
		//			dptr = new Texture();
		//			g_ObjectManager->AddObject(objectID, type, (ObjectPtr) dptr);
		//		}

		//		mustSerialize = true;
		//	}
		//	else
		//	{
		//		dptr = (Texture*) loadedObject;
		//	}

		//	char** aptr = (char**)ptr;
		//	*aptr = (char*)dptr;
		//	ptr = *(char**)ptr;

		//	if (mustSerialize)
		//	{
		//		Texture* val = (Texture *)ptr;
		//		val->SetObjectID(objectID);
		//		val->Serialize(false);
		//	}

		//	break;
		//}


		case LXType_Mesh:
		{
			int objectID;
			objectStream >> objectID;
			if (!g_ObjectManager->IsValidID(objectID))
			{
				break;
			}

			bool hardcodedId = g_ObjectManager->IsHardcodedID(objectID);
			if (*ptr == 0)
			{
				if (hardcodedId)
				{

				}
				else
				{
					Mesh* dptr = new Mesh();
					char** aptr = (char**)ptr;
					*aptr = (char*)dptr;
					ptr = *(char**)ptr;
				}
			}
			Mesh* val = (Mesh *) ptr;

			if (hardcodedId)
			{
				val = g_DefaultObjects->GetMeshFromID(objectID);
			}
			else
			{
				val->SetObjectID(objectID);
				val->Serialize(false);
			}

			break;
		}

	
		default:
		{
			break;
		}
	}
}

void Serializer::BackupData()
{
	std::string backupFolderName = StringUtils::GetTimestamp();

	std::experimental::filesystem::copy(g_PathObjects, g_PathObjectsBkp + backupFolderName);
}