#include "Editor.h"
#include "LigumX.h"
#include "GL.h"
#include "Renderer.h"
#include "World.h"
#include "SunLight.h"
#include "Model.h"
#include "EngineStats.h"
#include "BoundingBoxComponent.h"
#include "PostEffects.h"
#include "Mesh.h"
#include "Texture.h"
#include "SectorData.h"
#include "RenderDataManager.h"
#include "Sector.h"
#include "InputHandler.h"
#include "EngineSettings.h"

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
{ "ActiveTool", PIDX_ActiveTool, offsetof(Editor, m_ActiveTool), 0, LXType_EditorTool, false, LXType_None, PropertyFlags_Transient | PropertyFlags_Enum, 0, 0, }, 
{ "XYZMask", PIDX_XYZMask, offsetof(Editor, m_XYZMask), 0, LXType_glmvec4, false, LXType_None, PropertyFlags_Hidden | PropertyFlags_Transient | PropertyFlags_Adder, 0, 0, }, 
{ "PickedEntity", PIDX_PickedEntity, offsetof(Editor, m_PickedEntity), 0, LXType_Entity, true, LXType_None, PropertyFlags_Hidden | PropertyFlags_Transient, 0, 0, }, 
{ "PickedWorldPosition", PIDX_PickedWorldPosition, offsetof(Editor, m_PickedWorldPosition), 0, LXType_glmvec3, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "ManipulatorDragging", PIDX_ManipulatorDragging, offsetof(Editor, m_ManipulatorDragging), 0, LXType_bool, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "ManipulatorStartPosition", PIDX_ManipulatorStartPosition, offsetof(Editor, m_ManipulatorStartPosition), 0, LXType_glmvec3, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "SectorLoadingOffset", PIDX_SectorLoadingOffset, offsetof(Editor, m_SectorLoadingOffset), 0, LXType_glmivec2, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "PickingData", PIDX_PickingData, offsetof(Editor, m_PickingData), 0, LXType_glmvec4, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "EditingTerrain", PIDX_EditingTerrain, offsetof(Editor, m_EditingTerrain), 0, LXType_bool, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "TerrainErasureMode", PIDX_TerrainErasureMode, offsetof(Editor, m_TerrainErasureMode), 0, LXType_bool, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "TerrainBrushSize", PIDX_TerrainBrushSize, offsetof(Editor, m_TerrainBrushSize), 0, LXType_float, false, LXType_None, PropertyFlags_Adder, 0, 0, }, 
{ "PickingBufferSize", PIDX_PickingBufferSize, offsetof(Editor, m_PickingBufferSize), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
};
bool Editor::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
const std::string EnumValues_EditorTool[] = 
{
"None",
"TerrainHeight",
"TerrainSplatMap",
"EntityManipulator",
"SectorLoader",
};

const EditorTool Indirection_EditorTool[] =
{
	EditorTool_None,
	EditorTool_TerrainHeight,
	EditorTool_TerrainSplatMap,
	EditorTool_EntityManipulator,
	EditorTool_SectorLoader,
};

#pragma endregion  CLASS_SOURCE Editor

Editor::Editor()
	: m_SectorLoadingOffset(glm::ivec2(0, 0))
{
	
}

Editor::Editor(int objectID)
	: m_SectorLoadingOffset(glm::ivec2(0, 0))
{
	SetObjectID(objectID);
	Serialize(false);
}


void Editor::Initialize()
{
	Renderer* renderer = LigumX::GetRenderer();

	// Init Imgui
	ImGui_ImplGlfwGL3_Init(renderer->pWindow, true);

	m_SplatMapTexture = new Texture(48463);
}



bool fuzzyEquals(float a, float b, float tolerance)
{
	return fabs(a - b) < tolerance;
}

bool fuzzyEquals(glm::vec2 a, glm::vec2 b, float tolerance)
{
	return (fabs(a.x - b.x) < tolerance) && (fabs(a.y - b.y) < tolerance);
}


void Editor::UpdateManipulator()
{
	World* world = LigumX::GetInstance().getWorld();
	glm::vec3 worldPosition = glm::swizzle(m_PickingData, glm::R, glm::G, glm::B);
	const bool& mouseButton1Down = g_InputHandler->GetMouse1Pressed();
	glm::vec2 dragDistance = g_InputHandler->GetDragDistance();;

	if (!mouseButton1Down)
	{
		return;
	}

	if (!m_ManipulatorDragging || (m_XYZMask == glm::vec4(0, 0, 0, 0)))
	{
		float pickedID = m_PickingData[3];

		for (Entity* entity : world->GetDebugEntities())
		{
			// todo : proper int rendertarget; how does depth work then? do we care?
			if (fuzzyEquals(pickedID, entity->GetPickingID(), 0.005f))
			{
				if (entity->GetObjectID() == g_ObjectManager->DefaultManipulatorEntityID)
				{
					m_ManipulatorDragging = true;
					m_ManipulatorStartPosition = worldPosition;
				}
			}
		}

		for (Entity* entity : world->GetEntities())
		{
			// todo : proper int rendertarget; how does depth work then? do we care?
			if (fuzzyEquals(pickedID, entity->GetPickingID(), 0.005f))
			{
				m_PickedEntity = entity;


				g_DefaultObjects->DefaultManipulatorEntity->SetPosition(m_PickedEntity->GetPosition());

				// todo : ressuscitate this!
				//Renderer* renderer = LigumX::GetRenderer();
				//renderer->RenderEntityBB(m_PickedEntity);

				break;
			}
		}
	}
	else
	{
		World* world = LigumX::GetInstance().getWorld();

		float distance = dragDistance.x / 10.f;
		glm::vec3 toAdd = distance * glm::vec3(m_XYZMask);

		m_PickedEntity->AddToPosition(toAdd);
	}
}

void Editor::UpdateTerrainEditor()
{
	const bool& mouseButton1Down = g_InputHandler->GetMouse1Pressed();
	if (!mouseButton1Down)
	{
		return;
	}

	glm::vec3 worldPosition = glm::swizzle(m_PickingData, glm::R, glm::G, glm::B);
	glm::vec2 dragDistance = g_InputHandler->GetDragDistance();;

	Texture* tex = m_SplatMapTexture;
	glm::ivec2 texSize = tex->GetSize();
	int numTexels = texSize.x * texSize.y;
	int stride = 4;
	int numBytes = stride * numTexels;

	int brushWidth = m_TerrainBrushSize;
	int brushWidthSq = brushWidth * brushWidth;
	if (m_SplatMapData.size() != numBytes)
	{
		m_SplatMapData.resize(numBytes);
	}

	glm::vec2 screenDistance = dragDistance;
	screenDistance.y *= -1;

	glm::vec3 scale = m_PickedEntity->GetScale();
	glm::vec3 normalized = worldPosition / scale;

	glm::vec2 xyCoords = glm::vec2(normalized[0], normalized[1]);

	glm::ivec2 clickedTexel = glm::ivec2(xyCoords * glm::vec2(tex->GetSize()));
	glm::ivec2 startTexel = clickedTexel - glm::ivec2(brushWidth) / 2;
	startTexel = glm::max(startTexel, glm::ivec2(0, 0));
	startTexel = glm::min(startTexel, texSize - glm::ivec2(brushWidth));

	unsigned char* val = m_SplatMapData.data();

	int dataOffset = stride * (startTexel.y * tex->GetSize().x + startTexel.x);
	val += dataOffset;

	double maxVal = std::max(-screenDistance.y / 100, 0.f);

	glm::vec2 center = glm::vec2(0.5f, 0.5f);

	double maxHeight = maxVal * glm::length(center);
	double radius = 0.5f;

	for (int i = 0; i < brushWidth; ++i)
	{
		for (int j = 0; j < brushWidth; ++j)
		{
			int index = (int)(stride * (j * texSize.y + i));

			glm::vec2 localUV = glm::vec2(i, j) / glm::vec2(brushWidth);

			glm::vec2 centeredUV = localUV - center;
			double horizDist = glm::length(centeredUV);;

			float height = 0;

			if (horizDist < radius)
			{
				height = (~(0));
			}

			if (index < 0 || index > numBytes)
			{
				continue;
			}

			for (int c = 0; c < 4; ++c)
			{
				unsigned char& value = val[index + c];
				int toAdd = (int)m_XYZMask[c];

				if (GetTerrainErasureMode() && toAdd != 0)
				{
					value = 0;
				}
				else if (m_XYZMask.w == 0) // adding 
				{
					value += (value == 255) ? 0 : (char)toAdd;
				}
				else if (m_XYZMask.w == 1) // subtracting
				{
					value -= (value == 0) ? 0 : (char)toAdd;
				}

			}
		}
	}

	Renderer* renderer = LigumX::GetRenderer();
	renderer->Bind2DTexture(0, tex->GetHWObject());
	GLuint format = tex->GetFormat();
	GLuint type = tex->GetPixelType();

	//glTexSubImage2D(GL_TEXTURE_2D, 0, startTexel.x, startTexel.y, brushWidth, brushWidth, format, type, val);
	int startPoint = 0;
	glTexSubImage2D(GL_TEXTURE_2D, 0, startPoint, startPoint, texSize.x, texSize.y, format, type, m_SplatMapData.data());
	//tex->SaveToFile("C:\\temp\\output.png");

	renderer->Bind2DTexture(0, 0);

}

void Editor::UpdateSectorLoader()
{
	const bool& mouseButton1Down = g_InputHandler->GetMouse1Pressed();

	const glm::vec2& mousePosition = g_InputHandler->GetMousePosition();
	const glm::vec2& dragDistance = g_InputHandler->GetDragDistance();

	Renderer* renderer = LigumX::GetInstance().GetRenderer();
	glm::vec2 windowSize = glm::vec2(renderer->windowWidth, renderer->windowHeight);

	const glm::vec2 mouseNDC = mousePosition / windowSize;
	const glm::vec2 mouseScreen = mouseNDC * 2.f - glm::vec2(1, 1);

	const glm::vec4 screenSpaceRay = glm::normalize(glm::vec4(mouseScreen, 1.f, 0.f));

	const glm::mat4 cameraInverse = glm::inverse(renderer->GetDebugCamera()->GetViewProjectionMatrix());
	const glm::vec4 worldSpaceRay = glm::normalize(glm::mul(cameraInverse, screenSpaceRay));

	if (mouseButton1Down && dragDistance != glm::vec2(0,0))
	{
		PRINTVEC2(mouseNDC);
		PRINTVEC2(mouseScreen);

		PRINTVEC4(worldSpaceRay);
	}

	const glm::vec3 cameraPosition = renderer->GetDebugCamera()->GetPosition();
	const glm::vec3 cameraFront = renderer->GetDebugCamera()->GetFrontVector();

	const glm::vec3 planeNormal = glm::vec3(0, 0, 1);
	const glm::vec3 pointOnPlane = glm::vec3(0, 0, 0);

	float t = glm::dot(pointOnPlane - cameraPosition, planeNormal) / glm::dot(cameraFront, planeNormal);

	glm::vec3 worldPosition = cameraPosition + t * cameraFront;

	g_DefaultObjects->DefaultManipulatorEntity->SetPosition(worldPosition);



	World* world = LigumX::GetInstance().getWorld();

	if (m_Request.Finished())
	{
		curlThread.join();

		Sector* sector = new Sector(&m_Request);
		sector->SetOffsetIndex(glm::vec2(GetSectorLoadingOffset()));
		sector->loadData(SectorData::EOSMDataType::MAP);

		world->GetSectors().push_back(sector);
		world->sectors.push_back(sector);

		RenderDataManager::CreateWaysLines(world->sectors.back());

		m_Request.Reset();
	}
	else if (mouseButton1Down && m_Request.Ready())
	{
		//glm::ivec2 cornerIndex = g_EngineSettings->GetStartLonLat * 1e7;

		glm::vec2 startCoords = Sector::GetStartPosition(glm::vec2(worldPosition));
		glm::vec2 extent = glm::vec2(g_EngineSettings->GetExtent());
		glm::vec2 normalizedSectorIndex = Sector::GetNormalizedSectorIndex(glm::vec2(worldPosition));

		bool sectorAlreadyLoaded = false;

		for (Sector* sector : world->sectors)
		{
			if (fuzzyEquals(sector->GetPosition(), startCoords, 1e-2))
			{
				sectorAlreadyLoaded = sector->GetDataLoaded();
				break;
			}
		}

		if (!sectorAlreadyLoaded)
		{
			m_Request = CurlRequest(startCoords, extent);
			m_Request.Initialize();

			PRINTVEC2(normalizedSectorIndex);

			SetSectorLoadingOffset(glm::ivec2(normalizedSectorIndex));

			curlThread = std::thread(&CurlRequest::Execute, &m_Request);
		}
	}


}

void Editor::ApplyTool()
{
	switch (m_ActiveTool)
	{
		case EditorTool_EntityManipulator:
		{
			UpdateManipulator();
			break;
		}
		case EditorTool_TerrainSplatMap:
		{
			UpdateTerrainEditor();
			break;
		}
		case EditorTool_SectorLoader:
		{
			UpdateSectorLoader();
			break;
		}
		case EditorTool_None:
		default:
		{
			break;
		}
	}
}

void Editor::RenderPicking()
{
	Renderer* renderer = LigumX::GetRenderer();
	World* world = LigumX::GetInstance().getWorld();

	renderer->RenderPickingBuffer(m_Options->GetDebugDisplay());

	const glm::vec2& mousePosition = g_InputHandler->GetMousePosition();

	renderer->GetPickingData(mousePosition, m_PickingData);
}


void Editor::BeginImGUIWindow(unsigned int xSize, unsigned int ySize, ImGuiWindowFlags flags, bool open, const char* name)
{
	ImGui::SetNextWindowSize(ImVec2(xSize, ySize), ImGuiSetCond_FirstUseEver);

	if (!ImGui::Begin(name, &open, flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
}
void Editor::EndImGUIWindow()
{
	ImGui::End();
}

//const char* toCharPtr(glm::vec3 value, const char* variableName)
//{
//	char textBuffer[CHAR_BUFFER_SIZE];
//	sprintf(textBuffer, );
//	return textBuffer;
//}

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

void Editor::ShowProperty(bool* value, const char* name)
{
	ImGui::Checkbox(name, value);
}

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
		ImGui::SliderInt2(name, (int*)value, min, max);
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
		std::string sCopy = *((std::string*) ptr);
		static char buf_stdstring[64] = "";

		memset(buf_stdstring, 0, 64);
		memcpy(buf_stdstring, sCopy.c_str(), sCopy.size() * sizeof(char));

		if (ImGui::InputText(name, buf_stdstring, 64, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			*((std::string*) ptr) = std::string(buf_stdstring);
		}

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
	SHOW_PROPERTY_PTR(AABB)
	SHOW_PROPERTY_PTR(Entity)
	SHOW_PROPERTY_PTR(SunLight)
		//case LXType_Model:
		//{
		//	ShowPropertyGridTemplate<Model>((Model*) ptr, name);
		//	break;
		//}

	SHOW_PROPERTY_PTR(Texture)
	//SHOW_PROPERTY_PTR(Sector)
		BEGIN_PROPERTY(Sector)

		if ((int)ptr == 0) 
		{ 
			ShowGUIText(name); 
			ImGui::SameLine(); 
			if (ImGui::Button("New")) 
			{ 
				Sector* dptr = new Sector();
				*(char**)ptr = (char*)dptr; 
			} 
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
	SHOW_ENUM(EditorTool);

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

	float min = propertyData.m_MinValue;
	float max = propertyData.m_MaxValue;
	bool noneditable = propertyData.m_PropertyFlags & PropertyFlags_NonEditable;

	if (propertyData.m_Type == LXType_stdvector)
	{
		std::vector<char*>* v = (std::vector<char*>*) ptr;

		for (int i = 0; i < v->size(); ++i)
		{
			char displayName[100];
			sprintf(displayName, "%s[%d] : %s", propertyData.m_Name, i, object->GetName().c_str());

			ShowPropertyTemplate((*v)[i], displayName, propertyData.m_AssociatedType, min, max, noneditable);
		}

		ShowAddButton(v, propertyData.m_AssociatedType);
	}
	else
	{
		char* oldptr = ptr;
		ShowPropertyTemplate(ptr, propertyData.m_Name, propertyData.m_Type, min, max, noneditable);

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
	ImGui::Image((ImTextureID)object->GetHWObject(), ImVec2(50, 50));
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

					// todo :  the contents of this is shared with serializer.cpp and should go in its own function asap
					// rename ObjectManager to just ObjectManager?
					if (ShowEditableProperty(&objectID, propertyData.m_Name))
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

	World* world = LigumX::GetInstance().getWorld();
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
	World* world = LigumX::GetInstance().getWorld();

	ImGui_ImplGlfwGL3_NewFrame();
	if (g_Editor->GetOptions()->GetShowTestGUI())
	{
		ImGui::ShowTestWindow();
	}

	//if (g_Editor->GetOptions()->GetShowWorldWindow())
	{
		ImGui::PushID("WorldWindow");
		BeginImGUIWindow(1000, 700, ImGuiWindowFlags_MenuBar, 0, "Editor");

		ShowPropertyGridTemplate(g_InputHandler, "Input Handler");
		ShowPropertyGridTemplate(renderer->GetPostEffects(), "Post Effects");
		ShowPropertyGridTemplate(renderer->GetDebugCamera(), "Camera");
		ShowPropertyGridTemplate(world->GetSunLight(), "SunLight");
		ShowPropertyGridTemplate(world, "World");

		Editor* editor = this;
		ShowPropertyGridTemplate(editor, "Editor");

		// Menu
		if (ImGui::BeginMenuBar())
		{
			m_RenderingMenu = true;

			if (ImGui::BeginMenu("Menu"))
			{
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

				ImGui::EndMenu();
			}

			ShowPropertyGridTemplate<DisplayOptions>(renderer->GetDisplayOptions(), "Display options");
			ShowPropertyGridTemplate<EditorOptions>(GetOptions(), "Editor options");


			ImGui::EndMenuBar();

			m_RenderingMenu = false;
		}
		EndImGUIWindow();
		ImGui::PopID();

	}

	if (GetOptions()->GetShowEntityWindow())
	{
		ImGui::PushID("EntityWindow");

		BeginImGUIWindow(1000, 700, ImGuiWindowFlags_MenuBar, 0, "Entity");
		if (GetPickedEntity())
		{
			ShowPropertyGridObject<Entity>(GetPickedEntity(), "Entity");
		}
		else
		{
			ShowGUIText("No entity selected.");
		}
		EndImGUIWindow();
		ImGui::PopID();
	}

	if (GetOptions()->GetShowMaterialWindow())
	{
		ImGui::PushID("MaterialWindow");
		BeginImGUIWindow(1000, 700, ImGuiWindowFlags_MenuBar, 0, "Materials");

		if (GetPickedEntity())
		{
			int i = 0;
			for (Material*& material : GetPickedEntity()->GetModel()->GetMaterials())
			{
				ShowPropertyGridTemplate<Material>(material, ("Material #" + std::to_string(i++)).c_str());
			}
		}
		else
		{
			ShowGUIText("No entity selected.");
		}

		EndImGUIWindow();
		ImGui::PopID();
	}

	if (GetOptions()->GetShowObjectCreator())
	{
		BeginImGUIWindow(1000, 700, ImGuiWindowFlags_MenuBar, 0, "Object Creator");

		ShowObjectCreator<Entity>();
		ShowObjectCreator<Texture>();
		ShowObjectCreator<Material>();
		ShowObjectCreator<Model>();

		EndImGUIWindow();
	}

	if (GetOptions()->GetShowEngineStats())
	{
		BeginImGUIWindow(1000, 700, ImGuiWindowFlags_MenuBar, 0, "Engine Stats");

		ShowPropertyGridObject(g_EngineStats, "Engine Stats");

		EndImGUIWindow();
	}

	if (GetOptions()->GetShowObjectManager())
	{
		BeginImGUIWindow(1000, 700, ImGuiWindowFlags_MenuBar, 0, "Object Manager");

		ShowProperty<Texture>(g_ObjectManager->GetObjects(LXType_Texture), "Textures");
		ShowProperty<Mesh>(g_ObjectManager->GetObjects(LXType_Mesh), "Meshes");
		ShowProperty<Material>(g_ObjectManager->GetObjects(LXType_Material), "Materials");
		ShowProperty<Model>(g_ObjectManager->GetObjects(LXType_Model), "Models");

		EndImGUIWindow();
	}

	ImGui::Render();


}



void Editor::Render()
{
	RenderPicking();

	RenderImgui();

	ApplyTool();
}
