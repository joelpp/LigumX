#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "AABB.h"

#pragma region  FORWARD_DECLARATIONS Entity
#include "LXSystem.h"
class Serializer2;

class Entity;
class Model;
class Component;


#pragma endregion  FORWARD_DECLARATIONS Entity
class Model;

#pragma region  HEADER Entity
class Entity : public LXObject
{
public:
static const int ClassID = 1220590042;
static const LXType Type = LXType_Entity;
static constexpr const char* ClassName = "Entity";
typedef LXObject super;

bool GetVisible() { return m_Visible; }; 
void SetVisible(bool value) { m_Visible = value; }; 
glm::mat4& GetModelToWorldMatrix() { return m_ModelToWorldMatrix; }; 
void SetModelToWorldMatrix(const glm::mat4& value) { m_ModelToWorldMatrix = value; }; 
glm::vec3& GetPosition() { return m_Position; }; 
void SetPosition(const glm::vec3& value) { SetPosition_Callback(value); }; 
void SetPosition_Callback(const glm::vec3& value);
void AddTo_Position(glm::vec3 value) { m_Position += value; };
float GetRotationAngle() { return m_RotationAngle; }; 
void SetRotationAngle(float value) { SetRotationAngle_Callback(value); }; 
void SetRotationAngle_Callback(const float& value);
glm::vec3& GetRotationAxis() { return m_RotationAxis; }; 
void SetRotationAxis(const glm::vec3& value) { SetRotationAxis_Callback(value); }; 
void SetRotationAxis_Callback(const glm::vec3& value);
glm::vec3& GetScale() { return m_Scale; }; 
void SetScale(const glm::vec3& value) { SetScale_Callback(value); }; 
void SetScale_Callback(const glm::vec3& value);
bool GetHasMoved() { return m_HasMoved; }; 
void SetHasMoved(bool value) { SetHasMoved_Callback(value); }; 
void SetHasMoved_Callback(const bool& value);
float GetPickingID() { return m_PickingID; }; 
void SetPickingID(float value) { m_PickingID = value; }; 
Model*& GetModel() { return m_Model; }; 
void SetModel(Model* value) { SetModel_Callback(value); }; 
void SetModel_Callback(Model* value);
bool GetIsLight() { return m_IsLight; }; 
void SetIsLight(bool value) { m_IsLight = value; }; 
std::vector<Component*>& GetComponents() { return m_Components; }; 
void SetComponents(std::vector<Component*> value) { m_Components = value; }; 
void AddTo_Components(Component* value) { m_Components.push_back(value); };
private:
bool m_Visible = false;
glm::mat4 m_ModelToWorldMatrix;
glm::vec3 m_Position = glm::vec3(0, 0, 0);
float m_RotationAngle = 0.f;
glm::vec3 m_RotationAxis = glm::vec3(0, 0, 0);
glm::vec3 m_Scale = glm::vec3(0, 0, 0);
bool m_HasMoved = false;
float m_PickingID = 0.f;
Model* m_Model = nullptr;
bool m_IsLight = false;
std::vector<Component*> m_Components;
public:
static const int g_PropertyCount = 11;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_EntityPIDX
{
PIDX_Visible,
PIDX_ModelToWorldMatrix,
PIDX_Position,
PIDX_RotationAngle,
PIDX_RotationAxis,
PIDX_Scale,
PIDX_HasMoved,
PIDX_PickingID,
PIDX_Model,
PIDX_IsLight,
PIDX_Components,
};
void Serialize(Serializer2& serializer);
bool Serialize(bool writing);
void PostSerialization(bool writing, bool success);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER Entity
friend class PlayerController;
friend class AIController;
public :
    enum ControllerType {
        CONTROLLER_PLAYER,
        CONTROLLER_AI
    };

public:
    Entity();

    // @return : true if the entity has been updated
    void Update(double dt);
	void UpdateAABB();

    float GetForwardSpeed() const;
    glm::vec3 GetForwardVelocity() const;
    glm::vec3 GetLateralVelocity() const;

	void setPosition(glm::vec3 position) 
	{ 
		this->position = position; 
	}

public:
    // Entity features
    float mass;                        //!< Entity Mass
    float maxThrust;                 //!< Maximum thrust acceleration
    float maxForwardSpeed;      //!< Maximum atteignable forward speed
    float maxBackwardSpeed;    //!< Maximum atteignable backward speed

    glm::vec3 color;
    ControllerType type;

	template <typename T>
	T* GetComponent()
	{
		for (Component* component : m_Components)
		{
			if ( ((T*)component)->ClassID == T::ClassID)
			{
				return (T*)component;
			}
		}

		return nullptr;
	}

private:
    size_t entityIndex;

    // Entity physics
    float angle;
    int turning;
    float desiredSpeed;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 forwardVector;
    glm::vec3 rightVector;

};

#endif // ENTITY_H
