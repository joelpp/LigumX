#include "serializer.h"
#include "glm/glm.hpp"

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
		case LXType_glmvec2:
		{
			objectStream << name << std::endl;
			objectStream << *((glm::vec2*)ptr) << std::endl;
			break;
		}
		//case LXType_glmvec3:
		//{
		//ShowProperty((glm::vec3*) ptr, name, min, max);
		//break;
		//}
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


void Serializer::SerializePropertyIn(const char* ptr, const LXType& type, std::fstream& objectStream)
{
	switch (type)
	{
		case LXType_bool:
		{
			bool value;
			objectStream >> value;

			*((bool*)ptr) = value;
			break;
		}
		case LXType_float:
		{
			float value;
			objectStream >> value;

			*((float*)ptr) = value;
			break;
		}
		//case LXType_glmvec3:
		//{
		//ShowProperty((glm::vec3*) ptr, name, min, max);
		//break;
		//}
		case LXType_glmvec2:
		{
			float value;
			objectStream >> value;

			*((float*)ptr) = value;
			break;
		}
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

