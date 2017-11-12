#include "Editor.h"
#include "LigumX.h"
#include "GL.h"
#include "Renderer.h"
#include "World.h"

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
{ "ManipulatorDragging", PIDX_ManipulatorDragging, offsetof(Editor, m_ManipulatorDragging), 0, LXType_bool, false, LXType_None, PropertyFlags_Transient, 0, 0, }, 
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


bool fuzzyEquals(float a, float b, float tolerance)
{
	return fabs(a - b) < tolerance;
}


void Editor::RenderPicking()
{
	Renderer* renderer = LigumX::GetRenderer();
	World* world = LigumX::GetInstance().getWorld();

	renderer->RenderPickingBuffer(m_Options->GetDebugDisplay());


	float pickedID = renderer->GetPickedID(m_MousePosition);

	m_ManipulatorDragging &= m_MouseButton1Down;

	if (false /*manipulatorDragging*/)
	{
		glm::vec2 screenDistance = m_MousePosition - m_LastMousePosition;

		float distance = screenDistance.x / 10.f;
		glm::vec3 toAdd = distance * m_XYZMask;
		m_PickedEntity->AddToPosition(toAdd);
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
					break;
				}
			}

			// Update last click position
			m_LastMouseClickPosition = m_MouseClickPosition;
		}

	}

	// todo : this should be controlled by ManipulatorComponent
	if (m_PickedEntity)
	{
		g_DefaultObjects->DefaultManipulatorEntity->SetPosition(m_PickedEntity->GetPosition());
	}


	m_LastMousePosition = m_MousePosition;


}


void Editor::Render()
{
	RenderPicking();
}