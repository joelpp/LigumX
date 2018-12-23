#include "stdafx.h"

#include "glm/glm.hpp"
#include "Model.h"
#include "BoundingBoxComponent.h"

#pragma region  CLASS_SOURCE Entity

#include "Entity.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
#include "Model.h"
const ClassPropertyData Entity::g_Properties[] = 
{
{ "Visible", PIDX_Visible, offsetof(Entity, m_Visible), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ModelToWorldMatrix", PIDX_ModelToWorldMatrix, offsetof(Entity, m_ModelToWorldMatrix), 0, LXType_glmmat4, sizeof(glm::mat4), LXType_glmmat4, false, LXType_None, false, PropertyFlags_Hidden, 0, 0, 0,}, 
{ "Position", PIDX_Position, offsetof(Entity, m_Position), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, PropertyFlags_SetCallback | PropertyFlags_Adder, 0, 0, WriteSetFunction(Entity, Position, glm::vec3),}, 
{ "RotationAngle", PIDX_RotationAngle, offsetof(Entity, m_RotationAngle), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, PropertyFlags_SetCallback, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, WriteSetFunction(Entity, RotationAngle, float),}, 
{ "RotationAxis", PIDX_RotationAxis, offsetof(Entity, m_RotationAxis), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, PropertyFlags_SetCallback, 0, 0, WriteSetFunction(Entity, RotationAxis, glm::vec3),}, 
{ "Scale", PIDX_Scale, offsetof(Entity, m_Scale), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, PropertyFlags_SetCallback, 0, 0, WriteSetFunction(Entity, Scale, glm::vec3),}, 
{ "HasMoved", PIDX_HasMoved, offsetof(Entity, m_HasMoved), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, PropertyFlags_SetCallback, 0, 0, WriteSetFunction(Entity, HasMoved, bool),}, 
{ "PickingID", PIDX_PickingID, offsetof(Entity, m_PickingID), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "Model", PIDX_Model, offsetof(Entity, m_Model), 0, LXType_ObjectPtr, sizeof(Model*), LXType_Model, true, LXType_None, false, PropertyFlags_SetCallback, 0, 0, WriteSetFunction(Entity, Model, Model*),}, 
{ "IsLight", PIDX_IsLight, offsetof(Entity, m_IsLight), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Components", PIDX_Components, offsetof(Entity, m_Components), 0, LXType_stdvector, sizeof(std::vector<Component*>), LXType_stdvector, false, LXType_Component, true, 0, 0, 0, 0,}, 
};
bool Entity::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	PostSerialization(writing, success);
	return success;
}
bool Entity::ShowPropertyGrid()
{
	LXIMGUI_SHOW_BOOL("Visible", m_Visible);
	LXIMGUI_SHOW_VEC3("Position", m_Position, 0, 0);
	LXIMGUI_SHOW_FLOAT("RotationAngle", m_RotationAngle, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX);
	LXIMGUI_SHOW_VEC3("RotationAxis", m_RotationAxis, 0, 0);
	LXIMGUI_SHOW_VEC3("Scale", m_Scale, 0, 0);
	LXIMGUI_SHOW_BOOL("HasMoved", m_HasMoved);
	LXIMGUI_SHOW_FLOAT("PickingID", m_PickingID, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX);
	LXIMGUI_SHOW_OBJECTREF("Model", m_Model, Model);
	LXIMGUI_SHOW_BOOL("IsLight", m_IsLight);
	return true;
}

#pragma endregion  CLASS_SOURCE Entity

void Entity::PostSerialization(bool writing, bool success)
{
	if (success && !writing)
	{
		UpdateAABB();
	}
}

using namespace glm;

static float g_NextEntityPickingID = 1.f;

#define ENTITY_SIZE 0.0001f
#define POWER_MAG ENTITY_SIZE * 10.f
#define FRICTION_COEFF 0.5f

#define ENTITY_LINE_N 5 // 3 lines for vehicle, 2 lines for local space debug representation

Entity::Entity() 
	:  mass(1.f), 
	maxThrust(50.f), 
	maxForwardSpeed(50.f), 
	maxBackwardSpeed(-10.f),
	angle(0.f), 
	turning(0), 
	desiredSpeed(0.f),
	m_HasMoved(true)
{
	m_PickingID = g_NextEntityPickingID;
	g_NextEntityPickingID += 0.5f;

	m_RotationAxis = glm::vec3(1, 0, 0);
	m_Scale = glm::vec3(1, 1, 1);

	BoundingBoxComponent* bb = new BoundingBoxComponent();
	bb->SetParentEntity(this);
	m_Components.push_back(bb);

	SetObjectID(g_ObjectManager->GetNewObjectID());
}




float Entity::GetForwardSpeed() const {
    return dot(forwardVector, velocity);
}

vec3 Entity::GetForwardVelocity() const {
    return forwardVector * dot(forwardVector, velocity);
}

vec3 Entity::GetLateralVelocity() const {
    return rightVector * dot(rightVector, velocity);
}

void Entity::SetPosition_Callback(const glm::vec3& position)
{
	m_Position = position;
	SetHasMoved(true);
}

void Entity::SetHasMoved_Callback(const bool& value)
{
	m_HasMoved = value;
}

void Entity::SetScale_Callback(const glm::vec3& value)
{
	m_Scale = value;
	SetHasMoved(true);
}

void Entity::SetRotationAxis_Callback(const glm::vec3& value)
{
	m_RotationAxis = value;
	SetHasMoved(true);
}

void Entity::SetRotationAngle_Callback(const float& value)
{
	m_RotationAngle = value;
	SetHasMoved(true);
}

void Entity::SetModel_Callback(Model* model)
{
	m_Model = model;
	UpdateAABB();
}

void Entity::UpdateAABB()
{
	glm::vec3 min = glm::vec3(9999999);
	glm::vec3 max = glm::vec3(-9999999);;
	bool success = m_Model->GetMinMax(min, max);

	if (success)
	{
		BoundingBoxComponent* bbComponent = GetComponent<BoundingBoxComponent>();
		bbComponent->SetStartAndScale(min, max - min);
	}
}

void Entity::Update(double dt) 
{
	if (m_HasMoved)
	{
		glm::mat4x4 toWorld = glm::translate(glm::mat4(1.0), m_Position);
		toWorld = glm::rotate(toWorld, m_RotationAngle, m_RotationAxis);
		toWorld = glm::scale(toWorld, m_Scale);

		m_ModelToWorldMatrix = toWorld;

		UpdateAABB();
	}

	for (Component* component : m_Components)
	{
		component->Update();
	}

	SetHasMoved(false);
}











