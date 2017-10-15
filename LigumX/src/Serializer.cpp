#include "serializer.h"
#include "DisplayOptions.h"
#include "PostEffects.h"
#include "Camera.h"
#include "glm/glm.hpp"

#define SERIALIZE_PTR_OUT(type) \
case LXType_##type : \
{ \
##type* options = (##type*)ptr; \
options->Serialize(true); \
break; \
} \


void Serializer::SerializePropertyOut(const char* ptr, const char* name, const LXType& type, std::fstream& objectStream)
{
	switch (type)
	{
		case LXType_bool:
		{
			objectStream << name << std::endl;
			objectStream << *((bool*)ptr) << std::endl;
			break;
		}
		case LXType_float:
		{
			objectStream << name << std::endl;
			objectStream << *((float*)ptr) << std::endl;
			break;
		}
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
		//case LXType_Model:
		//{
		//ShowPropertyGridTemplate<Model>((Model*)ptr, name);
		//break;
		//}
		//case LXType_Material:
		//{
		//ShowPropertyGridTemplate<Material>((Material*)ptr, name);
		//break;
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

#define SERIALIZE_PRIMITIVE_IN(type) \
case LXType_ ##type : \
{ \
objectStream >> *((##type *)ptr); \
break; \
} \

#define SERIALIZE_PTR_IN(type) \
case LXType_ ##type : \
{ \
##type* val = (##type *)ptr; \
val->Serialize(false); \
break; \
} \

void Serializer::SerializePropertyIn(const char* ptr, const LXType& type, std::fstream& objectStream)
{
	switch (type)
	{
		SERIALIZE_PRIMITIVE_IN(bool)
		SERIALIZE_PRIMITIVE_IN(float)
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
		SERIALIZE_PTR_IN(DisplayOptions)
		SERIALIZE_PTR_IN(PostEffects)
		SERIALIZE_PTR_IN(Camera)
		//case LXType_PostEffects:
		//{
		//	PostEffects* postEffects = (PostEffects*)ptr;

		//	postEffects->Serialize(false);
		//	break;
		//}
		//case LXType_Model:
		//{
		//ShowPropertyGridTemplate<Model>((Model*)ptr, name);
		//break;
		//}
		//case LXType_Material:
		//{
		//ShowPropertyGridTemplate<Material>((Material*)ptr, name);
		//break;
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

