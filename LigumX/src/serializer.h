//#pragma once
//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include <string>
//
//template<typename T>
//void ShowPropertyGridMacro(T* object, const char* name)
//{
//	//ShowPropertyGrid(object, name);
//}
//
//void ShowProperty(bool* value, const char* name)
//{
//	//ImGui::Checkbox(name, value);
//}
//
//void ShowProperty(float* value, const char* name, float min, float max)
//{
//
//
//}
//
//void ShowProperty(glm::vec3* value, const char* name, float min, float max)
//{
//	////ShowVariableAsText(value, name);
//
//	//if (min == 0 && max == 0)
//	//{
//	//	ImGui::DragFloat3(name, (float*)value);
//	//}
//	//else
//	//{
//	//	ImGui::SliderFloat3(name, (float*)value, min, max);
//	//}
//}
//
//void ShowProperty(std::string* value, const char* name)
//{
//	//ShowGUIText(value, name);
//}
//
//void Renderer::ShowPropertyTemplate(const char* ptr, const char* name, const LXType& type, float min, float max)
//{
//	if (m_RenderingMenu && type == LXType_bool)
//	{
//		ImGui::MenuItem(name, NULL, (bool*)ptr);
//		return;
//	}
//
//	switch (type)
//	{
//	case LXType_bool:
//	{
//		ShowProperty((bool*)ptr, name);
//		break;
//	}
//	case LXType_float:
//	{
//		ShowProperty((float*)ptr, name, min, max);
//		break;
//	}
//	case LXType_glmvec3:
//	{
//		ShowProperty((glm::vec3*) ptr, name, min, max);
//		break;
//	}
//	case LXType_stdstring:
//	{
//		ShowProperty((std::string*) ptr, name);
//		break;
//	}
//	case LXType_Model:
//	{
//		ShowPropertyGridTemplate<Model>((Model*)ptr, name);
//		break;
//	}
//	case LXType_Material:
//	{
//		ShowPropertyGridTemplate<Material>((Material*)ptr, name);
//		break;
//	}
//	default:
//	{
//		break;
//	}
//	}
//}
//
//
//template<typename T>
//void Renderer::ShowGenericProperty(T* object, const ClassPropertyData& propertyData)
//{
//	if (propertyData.m_PropertyFlags & PropertyFlags_Hidden)
//	{
//		return;
//	}
//
//	char* ptr = (char*)object + propertyData.m_Offset;;
//
//	if (propertyData.IsAPointer)
//	{
//		ptr = *(char**)ptr;
//	}
//
//	float min = propertyData.m_MinValue;
//	float max = propertyData.m_MaxValue;
//
//	if (propertyData.m_Type == LXType_stdvector)
//	{
//		std::vector<char*>* v = (std::vector<char*>*) ptr;
//
//		for (int i = 0; i < v->size(); ++i)
//		{
//			char displayName[100];
//			sprintf(displayName, "%s[%d]", propertyData.m_Name, i);
//			ShowPropertyTemplate((*v)[i], displayName, propertyData.m_AssociatedType, min, max);
//		}
//	}
//	else
//	{
//		ShowPropertyTemplate(ptr, propertyData.m_Name, propertyData.m_Type, min, max);
//	}
//}
//
//
//template<typename T>
//void Renderer::ShowPropertyGridTemplate(T* object, const char* name)
//{
//	ImGui::PushID(name);
//
//
//	// one could have see having an enum to control how the property grid will be rendered
//	// i.e. menu, own window with treenode, treenode only
//	bool readyToDisplay = false;
//	if (m_RenderingMenu)
//	{
//		readyToDisplay = ImGui::BeginMenu(name);
//	}
//	else
//	{
//		readyToDisplay = ImGui::TreeNode(name);
//	}
//
//	if (readyToDisplay)
//	{
//		if (object == nullptr)
//		{
//			ShowGUIText("Object is null.");
//		}
//		else
//		{
//			for (const ClassPropertyData& propertyData : object->g_Properties)
//			{
//				ShowGenericProperty(object, propertyData);
//			}
//		}
//
//		if (m_RenderingMenu)
//		{
//			ImGui::EndMenu();
//		}
//		else
//		{
//			ImGui::TreePop();
//		}
//	}
//
//	ImGui::PopID();
//}