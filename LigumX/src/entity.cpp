

#include "glm/glm.hpp"
#include "Model.h"
#include "BoundingBoxComponent.h"

#pragma region  CLASS_SOURCE Entity

#include "Entity.h"
#include "serializer.h"
#include "Model.h"
#include "Component.h"
const ClassPropertyData Entity::g_Properties[] = 
{
{ "Visible", PIDX_Visible, offsetof(Entity, m_Visible), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ModelToWorldMatrix", PIDX_ModelToWorldMatrix, offsetof(Entity, m_ModelToWorldMatrix), 0, LXType_glmmat4, sizeof(glm::mat4), LXType_glmmat4, false, LXType_None, false, PropertyFlags_Hidden, 0, 0, 0,}, 
{ "Position", PIDX_Position, offsetof(Entity, m_Position), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, PropertyFlags_SetCallback | PropertyFlags_Adder, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, WriteSetFunction(Entity, Position, glm::vec3),}, 
{ "RotationAngle", PIDX_RotationAngle, offsetof(Entity, m_RotationAngle), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, PropertyFlags_SetCallback, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, WriteSetFunction(Entity, RotationAngle, float),}, 
{ "RotationAxis", PIDX_RotationAxis, offsetof(Entity, m_RotationAxis), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, PropertyFlags_SetCallback, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, WriteSetFunction(Entity, RotationAxis, glm::vec3),}, 
{ "Scale", PIDX_Scale, offsetof(Entity, m_Scale), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, PropertyFlags_SetCallback, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, WriteSetFunction(Entity, Scale, glm::vec3),}, 
{ "HasMoved", PIDX_HasMoved, offsetof(Entity, m_HasMoved), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, PropertyFlags_SetCallback | PropertyFlags_Transient, 0, 0, WriteSetFunction(Entity, HasMoved, bool),}, 
{ "PickingID", PIDX_PickingID, offsetof(Entity, m_PickingID), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, PropertyFlags_Transient | PropertyFlags_NoCloning, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "Model", PIDX_Model, offsetof(Entity, m_Model), 0, LXType_ObjectPtr, sizeof(Model*), LXType_Model, true, LXType_None, false, PropertyFlags_SetCallback, 0, 0, WriteSetFunction(Entity, Model, Model*),}, 
{ "IsLight", PIDX_IsLight, offsetof(Entity, m_IsLight), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Components", PIDX_Components, offsetof(Entity, m_Components), 0, LXType_stdvector, sizeof(std::vector<Component*>), LXType_stdvector, false, LXType_Component, true, 0, 0, 0, 0,}, 
{ "COMMAND_LoadModel", PIDX_COMMAND_LoadModel, offsetof(Entity, m_COMMAND_LoadModel), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, PropertyFlags_SetCallback | PropertyFlags_Transient, 0, 0, WriteSetFunction(Entity, COMMAND_LoadModel, bool),}, 
};
void Entity::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool(g_Properties[PIDX_Visible], m_Visible);
	serializer.SerializeVec3(g_Properties[PIDX_Position], m_Position);
	serializer.SerializeFloat(g_Properties[PIDX_RotationAngle], m_RotationAngle);
	serializer.SerializeVec3(g_Properties[PIDX_RotationAxis], m_RotationAxis);
	serializer.SerializeVec3(g_Properties[PIDX_Scale], m_Scale);
	serializer.SerializeObjectPtr(g_Properties[PIDX_Model], m_Model);
	serializer.SerializeBool(g_Properties[PIDX_IsLight], m_IsLight);
	serializer.SerializeVector(g_Properties[PIDX_Components], m_Components);
}
bool Entity::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	PostSerialization(writing, success);
	return success;
}
bool Entity::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Visible], &m_Visible  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Position], &m_Position , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_RotationAngle], &m_RotationAngle , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_RotationAxis], &m_RotationAxis , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Scale], &m_Scale , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_HasMoved], &m_HasMoved  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_PickingID], &m_PickingID , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_Model], &m_Model  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_IsLight], &m_IsLight  );
	ImguiHelpers::ShowProperty3(this, g_Properties[PIDX_Components], m_Components  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_COMMAND_LoadModel], &m_COMMAND_LoadModel  );
	return true;
}
void Entity::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	Entity* other = (Entity*) otherObj;
	other->SetVisible(m_Visible);
	other->SetModelToWorldMatrix(m_ModelToWorldMatrix);
	other->SetPosition(m_Position);
	other->SetRotationAngle(m_RotationAngle);
	other->SetRotationAxis(m_RotationAxis);
	other->SetScale(m_Scale);
	other->SetHasMoved(m_HasMoved);
	other->SetModel(m_Model);
	other->SetIsLight(m_IsLight);
	other->SetComponents(m_Components);
	other->SetCOMMAND_LoadModel(m_COMMAND_LoadModel);
}
const char* Entity::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE Entity

void Entity::PostSerialization(bool writing, bool success)
{
	SetHasMoved(true);
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
	
	bool success = m_Model && m_Model->GetMinMax(min, max);
	//glm::vec4 min4 = glm::vec4(min, 1.f);
	//glm::vec4 max4 = glm::vec4(max, 1.f);
	// todo jpp handle rotation


	if (success)
	{
		BoundingBoxComponent* bbComponent = GetComponent<BoundingBoxComponent>();
		if (bbComponent)
		{
			bbComponent->SetParentEntity(this);;
			bbComponent->SetStartAndScale(min, max - min);
		}
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
		if (component)
		{
			component->Update();
		}
	}

	SetHasMoved(false);
}

void Entity::SetCOMMAND_LoadModel_Callback(const bool& value)
{
	if (value )
	{
		m_Model->LoadModel();
	}
	// todo jpp : handle model unloading / reloading
}









