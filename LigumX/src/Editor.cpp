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
{ "MouseClickPosition", PIDX_MouseClickPosition, offsetof(Editor, m_MouseClickPosition), 0, LXType_glmvec2, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "LastMouseClickPosition", PIDX_LastMouseClickPosition, offsetof(Editor, m_LastMouseClickPosition), 0, LXType_glmvec2, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "LastMousePosition", PIDX_LastMousePosition, offsetof(Editor, m_LastMousePosition), 0, LXType_glmvec2, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "MousePosition", PIDX_MousePosition, offsetof(Editor, m_MousePosition), 0, LXType_glmvec2, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "MouseButton1Down", PIDX_MouseButton1Down, offsetof(Editor, m_MouseButton1Down), 0, LXType_bool, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "XYZMask", PIDX_XYZMask, offsetof(Editor, m_XYZMask), 0, LXType_glmvec3, false, LXType_None, PropertyFlags_Transient | PropertyFlags_Adder, 0, 0, }, 
{ "PickedEntity", PIDX_PickedEntity, offsetof(Editor, m_PickedEntity), 0, LXType_Entity, true, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "PickedWorldPosition", PIDX_PickedWorldPosition, offsetof(Editor, m_PickedWorldPosition), 0, LXType_glmvec3, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "ManipulatorDragging", PIDX_ManipulatorDragging, offsetof(Editor, m_ManipulatorDragging), 0, LXType_bool, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "MouseDragDistance", PIDX_MouseDragDistance, offsetof(Editor, m_MouseDragDistance), 0, LXType_glmvec2, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "PickedTexelOffset", PIDX_PickedTexelOffset, offsetof(Editor, m_PickedTexelOffset), 0, LXType_glmivec2, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
{ "TerrainBrushSize", PIDX_TerrainBrushSize, offsetof(Editor, m_TerrainBrushSize), 0, LXType_float, false, LXType_None, PropertyFlags_Adder, 0, 0, }, 
{ "PickingBufferSize", PIDX_PickingBufferSize, offsetof(Editor, m_PickingBufferSize), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
};
bool Editor::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

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
	Renderer* renderer = LigumX::GetRenderer();

	// Init Imgui
	ImGui_ImplGlfwGL3_Init(renderer->pWindow, true);
}



bool fuzzyEquals(float a, float b, float tolerance)
{
	return fabs(a - b) < tolerance;
}


void Editor::RenderPicking()
{
	Renderer* renderer = LigumX::GetRenderer();
	World* world = LigumX::GetInstance().getWorld();

	renderer->RenderPickingBuffer(m_Options->GetDebugDisplay());




	glm::vec4 pickingData;
	renderer->GetPickingData(m_MousePosition, pickingData);

	float pickedID = pickingData[3];
	glm::vec3 worldPosition = glm::swizzle(pickingData, glm::R, glm::G, glm::B);

	static bool editingTerrain = false;
	m_ManipulatorDragging &= m_MouseButton1Down;
	editingTerrain &= m_MouseButton1Down;

	if (editingTerrain || m_ManipulatorDragging)
	{

		if (m_ManipulatorDragging)
		{
			glm::vec2 screenDistance = m_MousePosition - m_LastMousePosition;
			float distance = screenDistance.x / 10.f;
			glm::vec3 toAdd = distance * m_XYZMask;

			m_PickedEntity->AddToPosition(toAdd);
		}
		else
		{
			glm::vec2 screenDistance = m_MousePosition - m_LastMouseClickPosition;
			screenDistance.y *= -1;

			glm::vec3 scale = m_PickedEntity->GetScale();
			glm::vec3 normalized = worldPosition / scale;

			PRINTVEC3(normalized);

			float width = m_TerrainBrushSize;
			glm::ivec2 offset = m_PickedTexelOffset;
			glm::vec2 clickedUV = glm::vec2(offset) / glm::vec2(width);

			std::vector<float> values(width * width );
			float* val = values.data();
			double maxVal = std::max(-screenDistance.y / 100, 0.f);

			glm::vec2 center = glm::vec2(0.5f, 0.5f);

			double maxHeight = maxVal * glm::length(center);
			double radius = 0.5f;

			for (int i = 0; i < width; ++i)
			{
				for (int j = 0; j < width; ++j)
				{
					glm::vec2 localUV = glm::vec2(i, j) / glm::vec2(width);

					glm::vec2 centeredUV = localUV - center;
					double horizDist = glm::length(centeredUV);;

					double height = 0;

					if (horizDist < radius)
					{
						height = maxVal * sqrt(pow(radius,2) - pow(horizDist, 2));
						//height = maxHeight - height;
					}

					val[(int)(i * width + j)] = (float)height;
				}
			}

			Texture* tex = m_PickedEntity->GetModel()->GetMaterials()[0]->GetHeightfieldTexture();
			renderer->Bind2DTexture(0, tex->GetHWObject());
			GLuint format = GL_RED;
			GLuint type = GL_FLOAT;

			glTexSubImage2D(GL_TEXTURE_2D, 0, offset.x, offset.y, width, width, format, type, values.data());

			PRINTVEC2(offset);
			PRINT(width);

			renderer->Bind2DTexture(0, 0);
		}
	}
	else
	{

		if (m_LastMouseClickPosition != m_MouseClickPosition)
		{
			for (Entity* entity : world->GetDebugEntities())
			{
				// todo : proper int rendertarget; how does depth work then? do we care?
				if (fuzzyEquals(pickedID, entity->GetPickingID(), 0.005f))
				{
					if (entity->GetObjectID() == g_ObjectManager->DefaultManipulatorEntityID)
					{
						m_ManipulatorDragging = true;
					}
				}
			}

			for (Entity* entity : world->GetEntities())
			{
				// todo : proper int rendertarget; how does depth work then? do we care?
				if (fuzzyEquals(pickedID, entity->GetPickingID(), 0.005f))
				{
					m_PickedEntity = entity;

					if (fuzzyEquals(9895.f, entity->GetObjectID(), 0.005f))
					{
						editingTerrain = true;
						SetMouseDragDistance(glm::vec2(0.f));

						glm::vec3 scale = m_PickedEntity->GetScale();
						glm::vec3 normalized = worldPosition / scale;

						glm::vec2 xyCoords = glm::vec2(normalized[0], normalized[1]);

						int width = m_TerrainBrushSize;

						glm::ivec2 offset = glm::ivec2(xyCoords * glm::vec2(450, 450)) - glm::ivec2(width) / 2;
						SetPickedTexelOffset(offset);
					}

					break;
				}
			}


			// Update last click position
			m_LastMouseClickPosition = m_MouseClickPosition;

			PRINTVEC4(pickingData);
		}


	}

	// todo : this should be controlled by ManipulatorComponent
	if (m_PickedEntity)
	{
		g_DefaultObjects->DefaultManipulatorEntity->SetPosition(m_PickedEntity->GetPosition());
	}


	m_LastMousePosition = m_MousePosition;

	renderer->RenderEntityBB(m_PickedEntity);
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
	case LXType_glmvec3:
	{
		ShowProperty((glm::vec3*) ptr, name, min, max);
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
	}

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
}
