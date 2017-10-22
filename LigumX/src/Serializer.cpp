#include "serializer.h"
#include "DisplayOptions.h"
#include "PostEffects.h"
#include "Camera.h"
#include "Model.h"
#include "Material.h"
#include "Component.h"
#include "Entity.h"
#include "glm/glm.hpp"

#define SERIALIZE_PTR_OUT(type) \
case LXType_##type : \
{ \
##type* val = (##type*) ptr; \
objectStream << name << std::endl; \
objectStream << val->GetObjectID() << std::endl; \
val->Serialize(true); \
break; \
} \

#define SERIALIZE_VALUE_OUT(type, name) \
case LXType_##type : \
{ \
objectStream << name << std::endl; \
objectStream << *((##type *)ptr) << std::endl; \
break; \
} \

#define SERIALIZE_VALUE_OUT_NS(nameSpace, type, name) \
case LXType_##nameSpace##type : \
{ \
objectStream << name << std::endl; \
objectStream << *((##nameSpace::##type *)ptr) << std::endl; \
break; \
} \


void Serializer::SerializePropertyOut(const char* ptr, const char* name, const LXType& type, std::fstream& objectStream)
{
	switch (type)
	{
		SERIALIZE_VALUE_OUT(int, name)
		SERIALIZE_VALUE_OUT(bool, name)
		SERIALIZE_VALUE_OUT(float, name)
		SERIALIZE_VALUE_OUT_NS(std, string, name)

		case LXType_glmvec3:
		{
			objectStream << name << std::endl;

			glm::vec3* value = (glm::vec3*) ptr;
			objectStream << value->x << std::endl;
			objectStream << value->y << std::endl;
			objectStream << value->z << std::endl;
			break;
		}
		case LXType_glmvec2:
		{
			objectStream << name << std::endl;

			glm::vec2* value = (glm::vec2*) ptr;
			objectStream << value->x << std::endl;
			objectStream << value->y << std::endl;
			break;
		}
		SERIALIZE_PTR_OUT(DisplayOptions);
		SERIALIZE_PTR_OUT(PostEffects);
		SERIALIZE_PTR_OUT(Camera);

		//case LXType_stdstring:
		//{
		//ShowProperty((std::string*) ptr, name);
		//break;
		//}
		case LXType_Model:
		{
			objectStream << name << std::endl;

			Model* model = (Model*) ptr;
			objectStream << model->GetObjectID() << std::endl;

			model->Serialize(true);
			break;
		}
		case LXType_Material:
		{
			Material* material = (Material*)ptr;

			objectStream << material->GetObjectID() << std::endl;

			material->Serialize(true);
			break;
		}

		case LXType_Entity:
		{
			Entity* entity = (Entity*)ptr;

			objectStream << entity->GetObjectID() << std::endl;

			entity->Serialize(true);
			break;
		}

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

#define SERIALIZE_PTR_IN(type) \
case LXType_##type : \
{ \
	if (*ptr == 0) \
	{ \
		##type* dptr = new type(); \
		char** aptr = (char**)ptr; \
		*aptr = (char*)dptr; \
		ptr = *(char**)ptr; \
	} \
	##type* val = (##type *)ptr; \
	int ObjectID; \
	objectStream >> ObjectID; \
	val->SetObjectID(ObjectID); \
	val->Serialize(false); \
	break; \
} \

#define SERIALIZE_PRIMITIVE_IN_NS(nameSpace, type) \
case LXType_ ##nameSpace##type : \
{ \
objectStream >> *((##nameSpace::##type *)ptr); \
break; \
} \

void Serializer::SerializePropertyIn(char*& ptr, const LXType& type, const LXType& associatedType, std::fstream& objectStream)
{
	switch (type)
	{
		SERIALIZE_PRIMITIVE_IN(int)
		SERIALIZE_PRIMITIVE_IN(bool)
		SERIALIZE_PRIMITIVE_IN(float)
		SERIALIZE_PRIMITIVE_IN_NS(std, string)
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
		// todo : camera

		//case LXType_DisplayOptions:
		//{
		//	//if ((int) *ptr == 0)
		//	//{
		//	//	DisplayOptions* newPtr = new DisplayOptions();

		//	//	for (int i = 0; i < 4; ++i)
		//	//	{
		//	//		*(ptr + i * sizeof(char)) = (char)(ptr + i * sizeof(char));
		//	//	}
		//	//}

		//	DisplayOptions* dptr = new DisplayOptions();

		//	char** aptr = (char**)ptr;
		//	*aptr = (char*)dptr;

		//	ptr = *(char**)ptr;

		//	DisplayOptions* val = (DisplayOptions *)ptr;

		//	int ObjectID;
		//	objectStream >> ObjectID;
		//	val->SetObjectID(ObjectID);
		//	val->Serialize(false);
		//	break;
		//}


		SERIALIZE_PTR_IN(DisplayOptions)
		SERIALIZE_PTR_IN(PostEffects)
		SERIALIZE_PTR_IN(Camera)
		SERIALIZE_PTR_IN(Material)

		//case LXType_Material:
		//{
		//	if (*ptr == 0)
		//	{
		//		Material* dptr = new Material();
		//		char** aptr = (char**)ptr;
		//		*aptr = (char*)dptr;
		//		ptr = *(char**)ptr;
		//	}
		//	
		//	Material* val = (Material *)ptr;
		//	int ObjectID;
		//	objectStream >> ObjectID;
		//	val->SetObjectID(ObjectID);
		//	val->Serialize(false);
		//	break;
		//}

		case LXType_Model:
		{
			Model* val = (Model *)ptr;

			//val = new Model();

			int objectID;
			objectStream >> objectID;

			val->SetObjectID(objectID);

			val->Serialize(false);
			break;
		}
		//case LXType_Material:
		//{
		//	Material** val = (Material **)ptr;

		//	if (*val == nullptr)
		//	{
		//		*val = new Material();
		//	}

		//	int objectID;
		//	objectStream >> objectID;

		//	(*val)->SetObjectID(objectID);

		//	(*val)->Serialize(false);
		//	break;
		//}
		case LXType_Entity:
		{
			Entity** val = (Entity **)ptr;

			if (*val == nullptr)
			{
				*val = new Entity();
			}

			int objectID;
			objectStream >> objectID;

			(*val)->SetObjectID(objectID);

			(*val)->Serialize(false);
			break;
		}

		//case LXType_Texture:
		//{
		//if (ptr)
		//{
		//ShowGUIText(name);
		//ImGui::Image((ImTextureID)((Texture*)ptr)->GetHWObject(), ImVec2(50, 50));
		//}

		//break;
		//}
		//case LXType_Component:
		//{
		//if (ptr)
		//{
		//ShowPropertyGridTemplate<BoundingBoxComponent>((BoundingBoxComponent*)ptr, name);
		//}

		//break;
		//}
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

