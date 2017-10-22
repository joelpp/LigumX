#include "stdafx.h"

#include "glm/glm.hpp"
#include "Model.h"
#include "BoundingBoxComponent.h"

#pragma region  CLASS_SOURCE Entity
#include "Entity.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectIdManager.h"
const ClassPropertyData Entity::g_Properties[] = 
{
{ "ObjectID", offsetof(Entity, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", offsetof(Entity, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "Position", offsetof(Entity, m_Position), 0, LXType_glmvec3, false, LXType_None, 0, 0, 0, }, 
{ "RotationAngle", offsetof(Entity, m_RotationAngle), 0, LXType_float, false, LXType_None, 0, 0, 0, }, 
{ "RotationAxis", offsetof(Entity, m_RotationAxis), 0, LXType_glmvec3, false, LXType_None, 0, 0, 0, }, 
{ "Scale", offsetof(Entity, m_Scale), 0, LXType_glmvec3, false, LXType_None, 0, 0, 0, }, 
{ "PickingID", offsetof(Entity, m_PickingID), 0, LXType_float, false, LXType_None, PropertyFlags_Hidden, 0, 0, }, 
{ "Model", offsetof(Entity, m_Model), 0, LXType_Model, true, LXType_None, 0, 0, 0, }, 
{ "IsLight", offsetof(Entity, m_IsLight), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "Components", offsetof(Entity, m_Components), 0, LXType_stdvector, false, LXType_Component, 0, 0, 0, }, 
};
void Entity::Serialize(bool writing)
{
	std::string basePath = "C:\\Users\\Joel\\Documents\\LigumX\\LigumX\\data\\objects\\";
	std::string fileName = "Entity_" + std::to_string(m_ObjectID) + ".LXobj";

	int fileMask = writing ? std::ios::out : std::ios::in;
	std::fstream objectStream(basePath + fileName, fileMask);

	if (objectStream.is_open())
	{
		Serializer::SerializeObject(this, objectStream, writing);
	}
}

#pragma endregion  CLASS_SOURCE Entity


using namespace glm;

static float g_NextEntityPickingID = 0.1;

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
	desiredSpeed(0.f)
{
	m_PickingID = g_NextEntityPickingID;
	g_NextEntityPickingID += 0.1;

	m_RotationAxis = glm::vec3(1, 0, 0);
	m_Scale = glm::vec3(1, 1, 1);

	BoundingBoxComponent* bb = new BoundingBoxComponent();
	bb->SetParentEntity(this);
	m_Components.push_back(bb);

	m_ObjectID = rand();

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

void Entity::Update(double dt) 
{
	glm::mat4x4 toWorld = glm::translate(glm::mat4(1.0), m_Position);
	toWorld = glm::rotate(toWorld, m_RotationAngle, m_RotationAxis);
	toWorld = glm::scale(toWorld, m_Scale);

	m_ModelToWorldMatrix = toWorld;

	for (Component* component : m_Components)
	{
		component->Update();
	}

	// todo : check out what adrien did for cars
	// , but for now it sleeps safely in source control
}











