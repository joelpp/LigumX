#include "Editor.h"
#include "LigumX.h"
#include "World.h"
#include "InputHandler.h"
#include "EngineSettings.h"

#include "LXError.h"
#include "Logging.h"
#include "StringUtils.h"
#include "MathUtils.h"

#include "GL.h"
#include "Renderer.h"
#include "Framebuffer.h"
#include "RenderDataManager.h"
#include "MainWindow.h"
#include "Mesh.h"
#include "Model.h"
#include "GUI.h"

#include "SunLight.h"
#include "EngineStats.h"
#include "BoundingBoxComponent.h"
#include "PostEffects.h"
#include "Texture.h"

#include "SectorManager.h"
#include "Sector.h"
#include "Heightfield.h"

#include "Node.h"
#include "Way.h"

#include "SectorTool.h"
#include "OSMTool.h"
#include "PickingTool.h"
#include "TerrainTool.h"


#pragma region  CLASS_SOURCE Editor
Editor* g_Editor;

#include "Editor.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData Editor::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(Editor, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", PIDX_Name, offsetof(Editor, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "Options", PIDX_Options, offsetof(Editor, m_Options), 0, LXType_EditorOptions, true, LXType_None, 0, 0, 0, }, 
{ "ActiveTool", PIDX_ActiveTool, offsetof(Editor, m_ActiveTool), 0, LXType_EEditorTool, false, LXType_None, PropertyFlags_Enum, 0, 0, }, 
{ "XYZMask", PIDX_XYZMask, offsetof(Editor, m_XYZMask), 0, LXType_glmvec4, false, LXType_None, PropertyFlags_Hidden | PropertyFlags_SetCallback | PropertyFlags_Transient | PropertyFlags_Adder, 0, 0, }, 
{ "ManipulatorDragging", PIDX_ManipulatorDragging, offsetof(Editor, m_ManipulatorDragging), 0, LXType_bool, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "ManipulatorStartPosition", PIDX_ManipulatorStartPosition, offsetof(Editor, m_ManipulatorStartPosition), 0, LXType_glmvec3, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "EditingTerrain", PIDX_EditingTerrain, offsetof(Editor, m_EditingTerrain), 0, LXType_bool, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "Tools", PIDX_Tools, offsetof(Editor, m_Tools), 0, LXType_stdvector, false, LXType_EditorTool, PropertyFlags_Transient, 0, 0, }, 
{ "PickingBufferSize", PIDX_PickingBufferSize, offsetof(Editor, m_PickingBufferSize), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "SelectedNode", PIDX_SelectedNode, offsetof(Editor, m_SelectedNode), 0, LXType_Node, true, LXType_None, 0, 0, 0, }, 
};
bool Editor::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
const std::string EnumValues_EEditorTool[] = 
{
"TerrainTool",
"EntityManipulator",
"SectorTool",
"OSMTool",
"PickingTool",
"None",
};

const EEditorTool Indirection_EEditorTool[] =
{
	EEditorTool_TerrainTool,
	EEditorTool_EntityManipulator,
	EEditorTool_SectorTool,
	EEditorTool_OSMTool,
	EEditorTool_PickingTool,
	EEditorTool_None,
};

#pragma endregion  CLASS_SOURCE Editor

Editor::Editor()
{
	
}

Editor::Editor(int objectID)
{
	SetObjectID(objectID);
	Serialize(false);
}


void Editor::Initialize()
{
	m_Tools.resize(EEditorTool_None);
	m_Tools[EEditorTool_OSMTool]		= new OSMTool();
	m_Tools[EEditorTool_PickingTool]	= new PickingTool();
	m_Tools[EEditorTool_SectorTool]		= new SectorTool();
	m_Tools[EEditorTool_TerrainTool]	= new TerrainTool();

	m_ToolDisplayToggles.resize(EEditorTool_None);
	m_ToolDisplayToggles[EEditorTool_OSMTool]		= 0;
	m_ToolDisplayToggles[EEditorTool_PickingTool]	= 0;
	m_ToolDisplayToggles[EEditorTool_SectorTool]	= 0;
	m_ToolDisplayToggles[EEditorTool_TerrainTool]	= 0;
}



void Editor::UpdateManipulator()
{
	World* world = LigumX::GetInstance().GetWorld();
	glm::vec3 worldPosition = GetPickingTool()->GetPickedWorldPosition();

	const bool& mouseButton1Down = g_InputHandler->GetMouse1Pressed();
	glm::vec2 dragDistance = g_InputHandler->GetDragDistance();;

	if (!mouseButton1Down)
	{
		return;
	}

	if (!m_ManipulatorDragging || (m_XYZMask == glm::vec4(0, 0, 0, 0)))
	{
		float pickedID = GetPickingTool()->GetPickedID();

		for (Entity* entity : world->GetDebugEntities())
		{
			// todo : proper int rendertarget; how does depth work then? do we care?
			if (MathUtils::FuzzyEquals(pickedID, entity->GetPickingID(), 0.05f))
			{
				if (entity->GetObjectID() == g_ObjectManager->DefaultManipulatorEntityID)
				{
					m_ManipulatorDragging = true;
					m_ManipulatorStartPosition = worldPosition;
				}
			}
		}

	}
	else
	{
		World* world = LigumX::GetInstance().GetWorld();

		float distance = dragDistance.x / 10.f;
		glm::vec3 toAdd = distance * glm::vec3(m_XYZMask);

		if (GetPickingTool()->GetPickedEntity())
		{
			GetPickingTool()->GetPickedEntity()->AddTo_Position(toAdd);
		}
	}
}

void Editor::UpdateTerrainEditor()
{
	//const bool& mouseButton1Down = g_InputHandler->GetMouse1Pressed();
	//if (!mouseButton1Down)
	//{
	//	return;
	//}

	//glm::vec3 worldPosition = GetPickingTool()->GetPickedWorldPosition();
	//glm::vec2 dragDistance = g_InputHandler->GetDragDistance();;

	//Texture* tex = m_SplatMapTexture;
	//glm::ivec2 texSize = tex->GetSize();
	//int numTexels = texSize.x * texSize.y;
	//int stride = 4;
	//int numBytes = stride * numTexels;

	//int brushWidth = (int) m_TerrainBrushSize;
	//int brushWidthSq = brushWidth * brushWidth;
	//if (m_SplatMapData.size() != numBytes)
	//{
	//	m_SplatMapData.resize(numBytes);
	//}

	//glm::vec2 screenDistance = dragDistance;
	//screenDistance.y *= -1;

	//glm::vec3 scale = GetPickingTool()->GetPickedEntity()->GetScale();
	//glm::vec3 normalized = worldPosition / scale;

	//glm::vec2 xyCoords = glm::vec2(normalized[0], normalized[1]);

	//glm::ivec2 clickedTexel = glm::ivec2(xyCoords * glm::vec2(tex->GetSize()));
	//glm::ivec2 startTexel = clickedTexel - glm::ivec2(brushWidth) / 2;
	//startTexel = glm::max(startTexel, glm::ivec2(0, 0));
	//startTexel = glm::min(startTexel, texSize - glm::ivec2(brushWidth));

	//unsigned char* val = m_SplatMapData.data();

	//int dataOffset = stride * (startTexel.y * tex->GetSize().x + startTexel.x);
	//val += dataOffset;

	//double maxVal = std::max(-screenDistance.y / 100, 0.f);

	//glm::vec2 center = glm::vec2(0.5f, 0.5f);

	//double maxHeight = maxVal * glm::length(center);
	//double radius = 0.5f;

	//for (int i = 0; i < brushWidth; ++i)
	//{
	//	for (int j = 0; j < brushWidth; ++j)
	//	{
	//		int index = (int)(stride * (j * texSize.y + i));

	//		glm::vec2 localUV = glm::vec2(i, j) / glm::vec2(brushWidth);

	//		glm::vec2 centeredUV = localUV - center;
	//		double horizDist = glm::length(centeredUV);;

	//		float height = 0;

	//		if (horizDist < radius)
	//		{
	//			height = (~(0));
	//		}

	//		if (index < 0 || index > numBytes)
	//		{
	//			continue;
	//		}

	//		for (int c = 0; c < 4; ++c)
	//		{
	//			unsigned char& value = val[index + c];
	//			int toAdd = (int)m_XYZMask[c];

	//			if (GetTerrainErasureMode() && toAdd != 0)
	//			{
	//				value = 0;
	//			}
	//			else if (m_XYZMask.w == 0) // adding 
	//			{
	//				value += (value == 255) ? 0 : (char)toAdd;
	//			}
	//			else if (m_XYZMask.w == 1) // subtracting
	//			{
	//				value -= (value == 0) ? 0 : (char)toAdd;
	//			}

	//		}
	//	}
	//}

	//Renderer* renderer = LigumX::GetRenderer();
	//renderer->Bind2DTexture(0, tex->GetHWObject());
	//GLuint format = tex->GetFormat();
	//GLuint type = tex->GetPixelType();

	////glTexSubImage2D(GL_TEXTURE_2D, 0, startTexel.x, startTexel.y, brushWidth, brushWidth, format, type, val);
	//int startPoint = 0;
	//glTexSubImage2D(GL_TEXTURE_2D, 0, startPoint, startPoint, texSize.x, texSize.y, format, type, m_SplatMapData.data());
	////tex->SaveToFile("C:\\temp\\output.png");

	//renderer->Bind2DTexture(0, 0);

}

void Editor::SetXYZMaskCallback(glm::vec4 value)
{
	m_XYZMask = value;

	GetTerrainTool()->SetXYZMask(value);
}



void Editor::ApplyTool()
{
	const bool& mouseButton1Down = g_InputHandler->GetMouse1Pressed();

	const glm::vec2& mousePosition = g_InputHandler->GetMousePosition();
	const glm::vec2& dragDistance = g_InputHandler->GetDragDistance();


	switch (m_ActiveTool)
	{
		case EEditorTool_EntityManipulator:
		{
			UpdateManipulator();
			break;
		}
		case EEditorTool_SectorTool:
		case EEditorTool_PickingTool:
		case EEditorTool_OSMTool:
		case EEditorTool_TerrainTool:
		{
			m_Tools[m_ActiveTool]->Process(mouseButton1Down, mousePosition, dragDistance);
			break;
		}
		case EEditorTool_None:
		default:
		{
			break;
		}
	}
}


void Editor::ShowVariableAsText(glm::vec3 variable, const char* variableName)
{
	ImGui::Text("%s : %f, %f, %f", variableName, variable.x, variable.y, variable.z);
}

void Editor::ShowVariableAsText(glm::vec3* variable, const char* variableName)
{
	ImGui::Text("%s : %f, %f, %f", variableName, variable->x, variable->y, variable->z);
}

void Editor::ShowGUIText(const char* text)
{
	ImGui::Text("%s", text);
}

void Editor::ShowGUIText(const std::string& text)
{
	ImGui::Text("%s", text.c_str());
}

void Editor::ShowGUIText(std::string* text)
{
	ImGui::Text("%s", text->c_str());
}

void Editor::ShowGUIText(std::string* text, const char* variableName)
{
	ImGui::Text("%s : %s", variableName, text->c_str());
}

void Editor::ShowGUIText(const std::string& text, const char* variableName)
{
	ImGui::Text("%s : %s", variableName, text.c_str());
}

void Editor::ShowVariableAsText(int variable, const char* variableName)
{
	ImGui::Text("%s : %d", variableName, variable);
}

void Editor::ShowVariableAsText(float variable, const char* variableName)
{
	ImGui::Text("%s : %f", variableName, variable);
}


#define SHOW_ADD_BUTTON(type) \
{ \
	case LXType_##type: \
	{ \
		if (ImGui::Button("+")) \
		{ \
			((std::vector<type *>*)vectorPtr)->push_back(new type()); \
		} \
	} \
}

void Editor::ShowAddButton(std::vector<char*>* vectorPtr, const LXType& type)
{
	switch (type)
	{
		SHOW_ADD_BUTTON(Model)
			SHOW_ADD_BUTTON(Material)
	}
}

#define GetProperty(type, object, data) (##type *)(##object) + data.m_Offset / sizeof(##type)

#define GetPropertyChar(object, data) (((char*)(##object)) + data.m_Offset)

#define GetPropertyPtrChar(object, data) (char**)(((char*)(##object)) + data.m_Offset)

#define GetPropertyVector(object, data) (##type *)(((char*)(##object)) + data.m_Offset); /*/ (2 * sizeof(##type))*/;

#define GetPropertyPtr(type, object, data) *((##type **)(##object) + data.m_Offset / sizeof(##type*))

#define ShowCheckbox(data, object) ImGui::Checkbox(data.m_Name, ((bool*)##object + data.m_Offset));

#define ShowFloatSlider(data, object, min, max) ImGui::SliderFloat(data.m_Name, (float*)(##object)+ data.m_Offset / 4, min, max, "%.3f");

#define ShowVec3(data, object) ShowVariableAsText(GetProperty(glm::vec3, object, data), data.m_Name); 

#define ShowString(data, object) ShowGUIText((std::string*)(##object) + data.m_Offset / sizeof(std::string), data.m_Name); 

#define ShowModel(data, object) ShowGUIText( "hey" + (GetProperty(Model, ##object, data))->GetName(), data.m_Name); 

template<typename T>
void Editor::ShowPropertyGridMacro(T* object, const char* name)
{
	ShowPropertyGrid(object, name);
}

void Editor::ShowProperty(int* value, const char* name)
{
	ShowVariableAsText(*value, name);
}

void Editor::ShowProperty(int* value, const char* name, int min, int max)
{
	ImGui::SliderInt(name, value, min, max);
}

void Editor::ShowProperty(bool* value, const char* name)
{
	ImGui::Checkbox(name, value);
}

void Editor::ShowProperty(bool& value, const char* name)
{
	ImGui::Checkbox(name, &value);
}
//
//void Editor::ShowProperty(std::vector<bool>::reference value, const char* name)
//{
//	ImGui::Checkbox(name, valuer);
//}


void Editor::ShowProperty(float* value, const char* name, float min, float max)
{
	if (min == 0 && max == 0)
	{
		ImGui::DragFloat(name, (float*)value);
	}
	else
	{
		ImGui::SliderFloat(name, value, min, max);
	}

}

void Editor::ShowProperty(glm::vec2* value, const char* name, float min, float max)
{
	if (min == 0 && max == 0)
	{
		ImGui::DragFloat2(name, (float*)value);
	}
	else
	{
		ImGui::SliderFloat2(name, (float*)value, min, max);
	}
}


void Editor::ShowProperty(glm::vec3* value, const char* name, float min, float max)
{
	if (min == 0 && max == 0)
	{
		ImGui::DragFloat3(name, (float*)value);
	}
	else
	{
		ImGui::SliderFloat3(name, (float*)value, min, max);
	}
}

void Editor::ShowProperty(glm::vec4* value, const char* name, float min, float max)
{
	if (min == 0 && max == 0)
	{
		ImGui::DragFloat4(name, (float*)value);
	}
	else
	{
		ImGui::SliderFloat4(name, (float*)value, min, max);
	}
}

void Editor::ShowProperty(glm::ivec2* value, const char* name, float min, float max)
{
	if (min == 0 && max == 0)
	{
		ImGui::DragInt2(name, (int*)value);
	}
	else
	{
		ImGui::SliderInt2(name, (int*)value, (int) min, (int) max);
	}
}

void Editor::ShowProperty(std::string* value, const char* name)
{
	ShowGUIText(value, name);
}

bool Editor::ShowEditableProperty(int* ptr, const char* name)
{
	int val = *((int*)ptr);
	std::string sCopy = std::to_string(val);

	static char buf_int[64] = "";
	memset(buf_int, 0, 64);
	memcpy(buf_int, sCopy.c_str(), sCopy.size() * sizeof(char));

	bool changed = ImGui::InputText(name, buf_int, 64, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal);
	if (changed)
	{
		*((int*)ptr) = std::atoi(buf_int);
	}

	return changed;
}

template <typename T>
void Editor::ShowProperty(std::map<int, char*>* map, const char* name)
{
	ImGui::PushID(name);
	if (ImGui::TreeNode(name))
	{
		for (auto it = map->begin(); it != map->end(); ++it)
		{
			T* obj = (T*)it->second;
			std::string label = obj->GetName() + " [" + std::to_string(it->first) + "]";
			ShowPropertyGridTemplate(obj, label.c_str());
		}
		ImGui::TreePop();
	}
	ImGui::PopID();
}

template <typename T>
void Editor::ShowProperty(std::map<int, T*>* map, const char* name)
{
	ImGui::PushID(name);
	if (ImGui::TreeNode(name))
	{
		for (auto it = map->begin(); it != map->end(); ++it)
		{
			T* obj = (T*)it->second;
			std::string label = obj->GetName() + " [" + std::to_string(it->first) + "]";
			ShowPropertyGridTemplate(obj, label.c_str());
		}
		ImGui::TreePop();
	}
	ImGui::PopID();
}

template <typename T>
void Editor::ShowProperty(std::unordered_map<int, char*>* map, const char* name)
{
	ImGui::PushID(name);
	if (ImGui::TreeNode(name))
	{
		for (auto it = map->begin(); it != map->end(); ++it)
		{
			T* obj = (T*)it->second;
			std::string label = obj->GetName() + " [" + std::to_string(it->first) + "]";
			ShowPropertyGridTemplate(obj, label.c_str());
		}
		ImGui::TreePop();
	}
	ImGui::PopID();
}

template <typename T>
void Editor::ShowProperty(std::unordered_map<int, T*>* map, const char* name)
{
	ImGui::PushID(name);
	if (ImGui::TreeNode(name))
	{
		for (auto it = map->begin(); it != map->end(); ++it)
		{
			T* obj = (T*)it->second;
			std::string label = obj->GetName() + " [" + std::to_string(it->first) + "]";
			ShowPropertyGridTemplate(obj, label.c_str());
		}
		ImGui::TreePop();
	}
	ImGui::PopID();
}

#define SHOW_ENUM(type) \
case LXType_##type: \
{ \
	ShowGUIText(name); \
	int* intPtr = (int*)ptr; \
	ImGui::SameLine(); \
	int index = *intPtr; \
	for (int i = 0; i < index; ++i) \
	{ \
		if (Indirection_##type[i] == index) \
		{ \
			index = i; \
			break; \
		} \
	} \
	if (ImGui::Button((EnumValues_##type)[index].c_str())) \
		ImGui::OpenPopup(name); \
	if (ImGui::BeginPopup(name)) \
	{ \
		ImGui::Text(#type); \
		ImGui::Separator(); \
		for (int i = 0; i < EnumLength_##type; i++) \
		{ \
			if (ImGui::Selectable((EnumValues_##type)[i].c_str())) \
			{ \
				*intPtr = Indirection_##type[i]; \
				break; \
			} \
		} \
		ImGui::EndPopup(); \
	} \
	break; \
}


#define SHOW_PROPERTY_PTR_INTERNAL(type) \
	if (*ptr == 0) \
	{ \
		ShowGUIText(name); \
		ImGui::SameLine(); \
		if (ImGui::Button("New")) \
		{ \
			type* dptr = new type(); \
			*(char**)ptr = (char*)dptr; \
		} \
	} \
	else \
	{ \
		ShowPropertyGridTemplate<##type>((##type *&) ptr, name); \
	} \

#define BEGIN_PROPERTY(type) \
case LXType_##type: \
{ 

#define END_PROPERTY(type) \
break; \
}

#define SHOW_PROPERTY_PTR(type) \
BEGIN_PROPERTY(type) \
SHOW_PROPERTY_PTR_INTERNAL(type) \
END_PROPERTY(type)

bool Editor::ShowPropertyTemplate(char*& ptr, const char* name, const LXType& type, float min, float max, bool noneditable)
{
	switch (type)
	{
	case LXType_int:
	{
		if (noneditable)
		{
			ShowProperty((int*)ptr, name);
		}
		else
		{
			ShowEditableProperty((int*)ptr, name);
		}
		break;
	}
	case LXType_bool:
	{
		if (m_RenderingMenu)
		{
			ImGui::MenuItem(name, NULL, (bool*)ptr);
		}
		else
		{
			ShowProperty((bool*)ptr, name);
		}
		break;
	}
	case LXType_float:
	{
		ShowProperty((float*)ptr, name, min, max);
		break;
	}
	case LXType_glmvec4:
	{
		ShowProperty((glm::vec4*) ptr, name, min, max);
		break;
	}
	case LXType_glmvec3:
	{
		ShowProperty((glm::vec3*) ptr, name, min, max);
		break;
	}
	case LXType_glmvec2:
	{
		ShowProperty((glm::vec2*) ptr, name, min, max);
		break;
	}
	case LXType_glmivec2:
	{
		ShowProperty((glm::ivec2*) ptr, name, min, max);
		break;

	}
	case LXType_stdstring:
	{
		const int maxFileNameSize = 512;

		std::string* s = (std::string*) ptr;
		ShowGUIText(*s);

		//std::string sCopy(s->c_str());

		//lxAssert(sCopy.size() <= maxFileNameSize);

		//static char buf_stdstring[maxFileNameSize];

		//memset(buf_stdstring, 0, maxFileNameSize);
		//memcpy(buf_stdstring, sCopy.c_str(), sCopy.size() * sizeof(char));

		//if (ImGui::InputText(name, buf_stdstring, maxFileNameSize, ImGuiInputTextFlags_EnterReturnsTrue))
		//{
		//	*((std::string*) ptr) = std::string(buf_stdstring);
		//}

		break;
	}

	//SHOW_PROPERTY_PTR(Model)
	case LXType_Model:
	{
		if (*ptr == 0)
		{
			ShowGUIText(name);
			if (ImGui::Button("New"))
			{
				Model* dptr = new Model();
				*(char**)ptr = (char*)dptr;
			}
		}
		else
		{

			ShowPropertyGridTemplate<Model>((Model *&)ptr, name);
		}
		break;
	}

	SHOW_PROPERTY_PTR(Material)
	SHOW_PROPERTY_PTR(Node)
	SHOW_PROPERTY_PTR(Way)
	SHOW_PROPERTY_PTR(AABB)
	SHOW_PROPERTY_PTR(Entity)
	SHOW_PROPERTY_PTR(SunLight)
	SHOW_PROPERTY_PTR(Heightfield)
		//case LXType_Model:
		//{
		//	ShowPropertyGridTemplate<Model>((Model*) ptr, name);
		//	break;
		//}

	SHOW_PROPERTY_PTR(Texture)
	//SHOW_PROPERTY_PTR(Sector)
		BEGIN_PROPERTY(Sector)

		// TODO : not entirely sure this comparison is good
		if (ptr == (char*) 0) 
		{ 
			ShowGUIText(name); 
			ImGui::SameLine(); 
		} 
		else 
		{ 
			ShowPropertyGridTemplate<Sector>((Sector *&) ptr, name);
		} 

		END_PROPERTY(Sector)

		//{
		//	BEGIN_PROPERTY(Texture);

		//	SHOW_PROPERTY_PTR_INTERNAL(Texture);

		//	if (*ptr != 0)
		//	{
		//		ImGui::Image((ImTextureID)((Texture*)ptr)->GetHWObject(), ImVec2(50, 50));
		//	}

		//	END_PROPERTY(Texture);
		//}

		//case LXType_Texture:
		//{
		//	if (ptr)
		//	{
		//		ShowGUIText(name);
		//		ImGui::Image((ImTextureID)((Texture*)ptr)->GetHWObject(), ImVec2(50, 50));
		//	}

		//	break;
		//}

	case LXType_Component:
	{
		if (ptr)
		{
			ShowPropertyGridTemplate<BoundingBoxComponent>((BoundingBoxComponent *&)ptr, name);
		}

		break;
	}


	SHOW_ENUM(ShaderFamily);
	SHOW_ENUM(GLPixelType);
	SHOW_ENUM(GLPixelFormat);
	SHOW_ENUM(EEditorTool);
	SHOW_ENUM(TerrainEditionMode);

	//case LXType_GLPixelType: \
	//{ \
	//ShowGUIText(name); \
	//int* intPtr = (int*)ptr; \
	//ImGui::SameLine(); \
	//int index = *intPtr; \
	//for (int i = 0; i < index; ++i)
	//{
	//	if (Indirection_GLPixelType[i] == index)
	//	{
	//		index = i;
	//		break;
	//	}
	//}
	//if (ImGui::Button((EnumValues_GLPixelType)[index].c_str())) \
	//	ImGui::OpenPopup("select"); \
	//if (ImGui::BeginPopup("select")) \
	//{ \
	//ImGui::Text("GLPixelType"); \
	//ImGui::Separator(); \
	//for (int i = 0; i < g_GLPixelTypeEnumLength; i++) \
	//{ \
	//if (ImGui::Selectable((EnumValues_GLPixelType)[i].c_str())) \
	//{ \
	//*intPtr = Indirection_GLPixelType[i]; \
	//} \
	//} \
	//ImGui::EndPopup(); \
	//} \
	//break; \
	//}

	/*
	case LXType_ShaderFamily:
	{
		ShowGUIText(name);

		int* intPtr = (int*)ptr;
		ImGui::SameLine();
		if (ImGui::Button(g_ShaderFamilyEnumValues[*intPtr].c_str()))
			ImGui::OpenPopup("select");
		if (ImGui::BeginPopup("select"))
		{
			ImGui::Text("ShaderFamily");
			ImGui::Separator();
			for (int i = 0; i < ShaderFamily_NumItems; i++)
			{
				if (ImGui::Selectable(g_ShaderFamilyEnumValues[i].c_str()))
				{
					*intPtr = i;
				}
			}

			ImGui::EndPopup();
		}


		break;
	}*/

	default:
	{
		break;
	}
	}

	return false;
}


template<typename T>
void Editor::ShowGenericProperty(T*& object, const ClassPropertyData& propertyData)
{
	if (propertyData.m_PropertyFlags & PropertyFlags_Hidden)
	{
		return;
	}

	char* ptr = (char*)object + propertyData.m_Offset;;

	if (propertyData.IsAPointer && (*ptr != 0))
	{
		ptr = *(char**)ptr;
	}

	//if (propertyData.IsAPointer && ((char*)(*ptr) == nullptr))
	//{
	//	ShowGUIText(propertyData.m_Name + std::string(" : nullptr"));
	//	return;
	//}

	std::string sanitizedPropertyName = StringUtils::SeparateByCapitalLetters(std::string(propertyData.m_Name));

	float min = propertyData.m_MinValue;
	float max = propertyData.m_MaxValue;
	bool noneditable = (propertyData.m_PropertyFlags & PropertyFlags_NonEditable) != 0;

	if (propertyData.m_Type == LXType_stdvector)
	{
		std::vector<char*>* v = (std::vector<char*>*) ptr;

		{
			const char* propertyName = sanitizedPropertyName.c_str();
			ImGui::PushID(propertyName);
			if (ImGui::TreeNode(propertyName))
			{
				for (int i = 0; i < v->size(); ++i)
				{
					char displayName[100];
					sprintf(displayName, "%s[%d]", object->GetName().c_str(), i);

					ShowPropertyTemplate((*v)[i], displayName, propertyData.m_AssociatedType, min, max, noneditable);
				}

				ImGui::TreePop();
			}
			ImGui::PopID();
		}

		ShowAddButton(v, propertyData.m_AssociatedType);
	}
	else
	{
		char* oldptr = ptr;
		ShowPropertyTemplate(ptr, sanitizedPropertyName.c_str(), propertyData.m_Type, min, max, noneditable);

		if (oldptr != ptr)
		{
			char** pptr = (char**)((char*)object + propertyData.m_Offset);
			*pptr = ptr;
		}

	}
}


template<typename T>
void Editor::EndShowPropertyGridObject(T*& object, const char* name)
{

}

void Editor::EndShowPropertyGridObject(Texture*& object, const char* name)
{
#pragma warning(disable:4312) // TODO : figure out what to do here
	ImGui::Image((ImTextureID)object->GetHWObject(), ImVec2(50, 50));
#pragma warning(default:4312)
}


template<typename T>
void Editor::ShowPropertyGridObject(T*& object, const char* name)
{
	if (object)
	{
		bool editing = false;
		for (const ClassPropertyData& propertyData : object->g_Properties)
		{
			bool display = !editing;
			if (g_ObjectManager->IsSupportedType(T::Type))
			{
				if (propertyData.m_Index == T::PIDX_ObjectID)
				{
					int objectID = object->GetObjectID();

					std::string sanitizedPropertyName = StringUtils::SeparateByCapitalLetters(std::string(propertyData.m_Name));
					// todo :  the contents of this is shared with serializer.cpp and should go in its own function asap
					// rename ObjectManager to just ObjectManager?
					if (ShowEditableProperty(&objectID, sanitizedPropertyName.c_str()))
					{
						ObjectPtr loadedObject = g_ObjectManager->FindObjectByID(objectID, T::Type, false);
						if (loadedObject == nullptr)
						{
							//PRINT("Object " + std::to_string(objectID) + " not found in ObjectManager. Trying to serialize from file.");
							std::cout << "Object " + std::to_string(objectID) + " not found in ObjectManager. Trying to serialize from file." << std::endl;
							T* newObject = new T();
							newObject->SetObjectID(objectID);
							bool success = newObject->Serialize(false);

							if (success)
							{
								object = newObject;
								g_ObjectManager->AddObject(newObject);
							}
						}
						else
						{
							object = (T*)loadedObject;
						}
					}

					editing = ImGui::IsItemActive();

					display = false;
				}
			}

			if (display)
			{
				ShowGenericProperty(object, propertyData);
			}
		}

		EndShowPropertyGridObject(object, name);
	}
	else
	{
		ShowGUIText("Object is null.");

	}
}

template<typename T>
bool Editor::ShowPropertyGridTemplate(T*& object, const char* name)
{
	ImGui::PushID(name);

	// one could have see having an enum to control how the property grid will be rendered
	// i.e. menu, own window with treenode, treenode only, __just the properties__
	bool readyToDisplay = false;
	if (m_RenderingMenu)
	{
		readyToDisplay = ImGui::BeginMenu(name);
	}
	else
	{
		readyToDisplay = ImGui::TreeNode(name);
	}

	if (readyToDisplay)
	{
		ShowPropertyGridObject(object, name);

		if (m_RenderingMenu)
		{
			ImGui::EndMenu();
		}
		else
		{
			ImGui::TreePop();
		}
	}

	ImGui::PopID();

	return readyToDisplay;
}



template<typename T>
void Editor::SaveObjectFromCreator(T* object)
{
	object->Serialize(true);
}

template<>
void Editor::SaveObjectFromCreator(Entity* newEntity)
{
	newEntity->GetModel()->Serialize(false);

	World* world = LigumX::GetInstance().GetWorld();
	world->GetEntities().push_back(newEntity);
}

template <typename T>
void Editor::ShowObjectCreator()
{
	static T* m_TempObject = new T();
	static int m_TempObjectID = g_ObjectManager->GetNewObjectID();
	ImGui::PushID(m_TempObjectID);

	if (ShowPropertyGridTemplate<T>(m_TempObject, ("New " + std::string(T::ClassName)).c_str()))
	{
		if (ImGui::Button("Save"))
		{
			SaveObjectFromCreator<T>(m_TempObject);
		}

		if (ImGui::Button("Reset"))
		{
			m_TempObject = new T();
		}

		ShowEditableProperty(&m_TempObjectID, "ID to load");

		if (ImGui::Button("Load"))
		{
			m_TempObject->SetObjectID(m_TempObjectID);
			m_TempObject->Serialize(false);
		}

	}


	ImGui::PopID();
}

void Editor::BackupData()
{
	if (g_Editor->GetOptions()->GetBackupDataOnSave())
	{
		g_Serializer->BackupData();
	}
}

template <typename T>
void Editor::TrySaveObject(T* object)
{
	if (!g_Editor->GetOptions()->GetSaveDisabled())
	{
		BackupData();
		object->Serialize(true);
	}
	else
	{
		// todo : rebuild the logging system to use imgui
		/*PRINTSTRING("Output serialization is currently disabled.")*/

		std::cout << "Output serialization is currently disabled." << std::endl;
	}
}



void Editor::RenderImgui()
{
	Renderer* renderer = LigumX::GetRenderer();
	World* world = LigumX::GetInstance().GetWorld();

	ImGui_ImplGlfwGL3_NewFrame();
	if (g_Editor->GetOptions()->GetShowTestGUI())
	{
		ImGui::ShowTestWindow();
	}
	// Menu
	if (ImGui::BeginMainMenuBar())
	{
		m_RenderingMenu = true;

		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem("Reset World"))
			{
				LigumX::GetInstance().ResetWorld();
			}
			if (ImGui::MenuItem("Reset Sectors"))
			{
				LigumX::GetInstance().GetWorld()->ResetSectors();
			}
			if (ImGui::MenuItem("Save Editor"))
			{
				TrySaveObject(g_Editor);
			}
			if (ImGui::MenuItem("Save renderer"))
			{
				TrySaveObject(this);
			}
			if (ImGui::MenuItem("Save world"))
			{
				TrySaveObject(world);
			}
			if (ImGui::MenuItem("Quit"))
			{
				LigumX::GetInstance().Shutdown();
			}

			ImGui::EndMenu();
		}

		ShowPropertyGridTemplate<DisplayOptions>(renderer->GetDisplayOptions(), "Display options");
		ShowPropertyGridTemplate<EditorOptions>(GetOptions(), "Editor options");

		{
			const char* name = "Tools";


			if (ImGui::BeginMenu(name))
			{
				ImGui::PushID(name);

				std::vector<int>& displayToggles = m_ToolDisplayToggles;
				for (int i = 0; i < displayToggles.size(); ++i)
				{
					bool b = displayToggles[i] == 1 ? true : false;
					ShowProperty(&b, EnumValues_EEditorTool[i].c_str());
					displayToggles[i] = b ? 1 : 0;

					lxAssert(displayToggles[i] == 0 || displayToggles[i] == 1);
				}

				ImGui::PopID();
				ImGui::EndMenu();
			}
		}

		ShowPropertyGridTemplate<EngineSettings>(g_EngineSettings, "Engine Settings");


		ImGui::EndMainMenuBar();

		m_RenderingMenu = false;
	}
	//if (g_Editor->GetOptions()->GetShowWorldWindow())
	{
		ImGui::PushID("WorldWindow");
		g_GUI->BeginWindow(1000, 700, 0, 0, "Editor");

		ShowPropertyGridTemplate(g_InputHandler, "Input Handler");
		ShowPropertyGridTemplate(renderer->GetPostEffects(), "Post Effects");
		ShowPropertyGridTemplate(renderer->GetDebugCamera(), "Camera");
		ShowPropertyGridTemplate(world->GetSunLight(), "SunLight");
		ShowPropertyGridTemplate(world, "World");

		Editor* editor = this;
		ShowPropertyGridTemplate(editor, "Editor");

		
		g_GUI->EndWindow();
		ImGui::PopID();

	}

	if (GetOptions()->GetShowEntityWindow())
	{
		ImGui::PushID("EntityWindow");

		g_GUI->BeginWindow(1000, 700, ImGuiWindowFlags_MenuBar, 0, "Entity");

		if (GetPickingTool()->GetPickedEntity())
		{
			ShowPropertyGridObject<Entity>(GetPickingTool()->GetPickedEntity(), "Entity");
		}
		else
		{
			ShowGUIText("No entity selected.");
		}

		g_GUI->EndWindow();
		ImGui::PopID();
	}

	if (GetOptions()->GetShowMaterialWindow())
	{
		ImGui::PushID("MaterialWindow");
		g_GUI->BeginWindow(1000, 700, ImGuiWindowFlags_MenuBar, 0, "Materials");

		if (GetPickingTool()->GetPickedEntity())
		{
			int i = 0;
			for (Material*& material : GetPickingTool()->GetPickedEntity()->GetModel()->GetMaterials())
			{
				ShowPropertyGridTemplate<Material>(material, ("Material #" + std::to_string(i++)).c_str());
			}
		}
		else
		{
			ShowGUIText("No entity selected.");
		}
		
		g_GUI->EndWindow();
		ImGui::PopID();
	}

	if (GetOptions()->GetShowObjectCreator())
	{
		g_GUI->BeginWindow(1000, 700, ImGuiWindowFlags_MenuBar, 0, "Object Creator");

		ShowObjectCreator<Entity>();
		ShowObjectCreator<Texture>();
		ShowObjectCreator<Material>();
		ShowObjectCreator<Model>();

		g_GUI->EndWindow();
	}

	if (GetOptions()->GetShowEngineStats())
	{
		g_GUI->BeginWindow(1000, 700, ImGuiWindowFlags_MenuBar, 0, "Engine Stats");

		ShowPropertyGridObject(g_EngineStats, "Engine Stats");

		g_GUI->EndWindow();
	}

	if (GetOptions()->GetShowObjectManager())
	{
		g_GUI->BeginWindow(1000, 700, ImGuiWindowFlags_MenuBar, 0, "Object Manager");

		ShowProperty<Texture>(g_ObjectManager->GetObjects(LXType_Texture), "Textures");
		ShowProperty<Mesh>(g_ObjectManager->GetObjects(LXType_Mesh), "Meshes");
		ShowProperty<Material>(g_ObjectManager->GetObjects(LXType_Material), "Materials");
		ShowProperty<Model>(g_ObjectManager->GetObjects(LXType_Model), "Models");

		g_GUI->EndWindow();
	}

	if (m_Options->GetDisplaySectorTool())
	{
		g_GUI->BeginWindow(1000, 700, 0, 0, "Sector Tool");
		SectorTool* sectorTool = GetSectorTool();

		ShowPropertyGridObject(sectorTool, "Sector Tool");

		{
			const char* name = "Display Toggles";
			ImGui::PushID(name);
			if (ImGui::TreeNode(name))
			{
				std::vector<int>& displayToggles = sectorTool->GetWayDisplayToggles();
				for (int i = 0; i < displayToggles.size(); ++i)
				{
					bool b = displayToggles[i] == 1 ? true : false;
					ShowProperty(&b, EnumValues_OSMElementType[i].c_str());
					displayToggles[i] = b ? 1 : 0;

					lxAssert(displayToggles[i] == 0 || displayToggles[i] == 1);
				}

				ImGui::TreePop();
			}
			ImGui::PopID();
		}

		{
			const char* name = "Display Colors";
			ImGui::PushID(name);
			if (ImGui::TreeNode(name))
			{
				std::vector<glm::vec3>& displayColors = sectorTool->GetWayDebugColors();
				for (int i = 0; i < displayColors.size(); ++i)
				{
					ShowProperty(&(displayColors[i]), EnumValues_OSMElementType[i].c_str(), 0, 1);
				}

				ImGui::TreePop();
			}
			ImGui::PopID();
		}

		ShowProperty<Way>(&(g_SectorManager->m_AllWaysPtr), "Ways");

		g_GUI->EndWindow();
	}


	if (m_ToolDisplayToggles[EEditorTool_TerrainTool] != 0)
	{
		g_GUI->BeginWindow(1000, 700, 0, 0, "Terrain Tool");

		TerrainTool* terrainTool = GetTerrainTool();
		ShowPropertyGridObject(terrainTool, "Terrain Tool");

		g_GUI->EndWindow();
	}

	
	if (m_Options->GetDisplayOSMTool())
	{
		g_GUI->BeginWindow(1000, 700, 0, 0, "OSM Tool");

		OSMTool* osmTool = GetOSMTool();
		ShowPropertyGridObject(osmTool, "OSM Tool");

		g_GUI->EndWindow();
	}

	if (m_Options->GetDisplayPickingTool())
	{
		g_GUI->BeginWindow(1000, 700, 0, 0, "Picking Tool");

		PickingTool* pickingTool = GetPickingTool();
		ShowPropertyGridObject(pickingTool, "Picking Tool");

		g_GUI->EndWindow();
	}

	if (m_Options->GetDisplayFramebufferTool())
	{
		g_GUI->BeginWindow(1000, 700, 0, 0, "Framebuffer Tool");

		ImVec2 size = ImGui::GetWindowSize();
		size.x -= 25;
		size.y -= 25;

		ImGui::Image((ImTextureID) renderer->GetFramebuffer(FramebufferType_Picking)->GetColorTexture(0), size);

		g_GUI->EndWindow();
	}


	ImGui::Render();
}

void Editor::ProcessScrolling()
{
	Camera* activeCamera = Renderer::GetInstance().GetDebugCamera();
	const glm::vec2& scrolling = g_InputHandler->GetMouseScroll();
	glm::vec3 moveVector;

	if (GetPickingTool()->GetPickedID() != 0)
	{
		float closeUp = m_Options->GetMouseScrollEntityCloseupPercent() / 100.f;
		float entityDistance = closeUp * GetPickingTool()->GetPickedDepth();
		moveVector = activeCamera->GetFrontVector() * scrolling.y * entityDistance;
	}
	else
	{
		moveVector = activeCamera->GetFrontVector() * scrolling.y * m_Options->GetMouseScrollCameraSpeed();
	}

	activeCamera->AddTo_Position(-moveVector);
}

void Editor::RenderTools()
{
	((SectorTool*)m_Tools[EEditorTool_SectorTool])->Display();

	DisplayAxisGizmo();
}

void Editor::DisplayAxisGizmo()
{
	if (!m_Options->GetDisplayAxisGizmo())
	{
		return;
	}

	Renderer::GetInstance().RenderAxisGizmo();
}


void Editor::Render()
{
	GetPickingTool()->UpdatePickingData();

	ApplyTool();

	ProcessScrolling();

	RenderImgui();
}


void Editor::DisplayActiveTool()
{

}
