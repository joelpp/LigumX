#include "stdafx.h"

#include "glm/glm.hpp"
#include "Model.h"
#include "BoundingBoxComponent.h"

#pragma region  CLASS_SOURCE Entity

#include "Entity.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData Entity::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(Entity, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, 0,}, 
{ "Name", PIDX_Name, offsetof(Entity, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, 0,}, 
{ "Visible", PIDX_Visible, offsetof(Entity, m_Visible), 0, LXType_bool, false, LXType_None, 0, 0, 0, 0,}, 
{ "ModelToWorldMatrix", PIDX_ModelToWorldMatrix, offsetof(Entity, m_ModelToWorldMatrix), 0, LXType_glmmat4, false, LXType_None, PropertyFlags_Hidden, 0, 0, 0,}, 
{ "Position", PIDX_Position, offsetof(Entity, m_Position), 0, LXType_glmvec3, false, LXType_None, PropertyFlags_SetCallback | PropertyFlags_Adder, 0, 0, WriteSetFunction(Entity, Position, glm::vec3),}, 
{ "RotationAngle", PIDX_RotationAngle, offsetof(Entity, m_RotationAngle), 0, LXType_float, false, LXType_None, PropertyFlags_SetCallback, 0, 0, 0,}, 
{ "RotationAxis", PIDX_RotationAxis, offsetof(Entity, m_RotationAxis), 0, LXType_glmvec3, false, LXType_None, PropertyFlags_SetCallback, 0, 0, WriteSetFunction(Entity, RotationAxis, glm::vec3),}, 
{ "Scale", PIDX_Scale, offsetof(Entity, m_Scale), 0, LXType_glmvec3, false, LXType_None, PropertyFlags_SetCallback, 0, 0, WriteSetFunction(Entity, Scale, glm::vec3),}, 
{ "PreviousPosition", PIDX_PreviousPosition, offsetof(Entity, m_PreviousPosition), 0, LXType_glmvec3, false, LXType_None, 0, 0, 0, 0,}, 
{ "PreviousScale", PIDX_PreviousScale, offsetof(Entity, m_PreviousScale), 0, LXType_glmvec3, false, LXType_None, 0, 0, 0, 0,}, 
{ "PreviousRotationAngle", PIDX_PreviousRotationAngle, offsetof(Entity, m_PreviousRotationAngle), 0, LXType_float, false, LXType_None, 0, 0, 0, 0,}, 
{ "PreviousRotationAxis", PIDX_PreviousRotationAxis, offsetof(Entity, m_PreviousRotationAxis), 0, LXType_glmvec3, false, LXType_None, 0, 0, 0, 0,}, 
{ "HasMoved", PIDX_HasMoved, offsetof(Entity, m_HasMoved), 0, LXType_bool, false, LXType_None, PropertyFlags_SetCallback, 0, 0, 0,}, 
{ "PickingID", PIDX_PickingID, offsetof(Entity, m_PickingID), 0, LXType_float, false, LXType_None, 0, 0, 0, 0,}, 
{ "Model", PIDX_Model, offsetof(Entity, m_Model), 0, LXType_Model, true, LXType_None, PropertyFlags_SetCallback, 0, 0, 0,}, 
{ "IsLight", PIDX_IsLight, offsetof(Entity, m_IsLight), 0, LXType_bool, false, LXType_None, 0, 0, 0, 0,}, 
{ "Components", PIDX_Components, offsetof(Entity, m_Components), 0, LXType_stdvector, false, LXType_Component, 0, 0, 0, 0,}, 
};
bool Entity::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	PostSerialization(writing, success);
	return success;
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

	m_ObjectID = g_ObjectManager->GetNewObjectID();

	//m_Model = new Model();
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

void Entity::SetPositionCallback(const glm::vec3& position)
{
	m_Position = position;
	SetHasMoved(true);
}

void Entity::SetHasMovedCallback(const bool& value)
{
	m_HasMoved = value;
}

void Entity::SetScaleCallback(const glm::vec3& value)
{
	m_Scale = value;
	SetHasMoved(true);
}

void Entity::SetRotationAxisCallback(const glm::vec3& value)
{
	m_RotationAxis = value;
	SetHasMoved(true);
}

void Entity::SetRotationAngleCallback(const float& value)
{
	m_RotationAngle = value;
	SetHasMoved(true);
}

void Entity::SetModelCallback(Model* model)
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

// horrible hack but as long as I can't detect imgui inputs that's what happens.
void Entity::CheckHasMoved()
{
	bool hasMoved = false;

	if (m_Position != m_PreviousPosition)
	{
		m_PreviousPosition = m_Position;
		hasMoved = true;
	}
	if (m_RotationAngle != m_PreviousRotationAngle)
	{
		m_PreviousRotationAngle = m_RotationAngle;
		hasMoved = true;
	}
	if (m_RotationAxis != m_PreviousRotationAxis)
	{
		m_PreviousRotationAxis = m_RotationAxis;
		hasMoved = true;
	}
	if (m_Scale != m_PreviousScale)
	{
		m_PreviousScale = m_Scale;
		hasMoved = true;
	}

	SetHasMoved(hasMoved);
}

void Entity::Update(double dt) 
{
	//CheckHasMoved();

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











