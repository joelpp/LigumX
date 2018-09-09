#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
//#include "glfw\include\GLFW\glfw3.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "AABB.h"

#pragma region  FORWARD_DECLARATIONS Entity
#include "property.h"

class Entity;
class Model;
class Component;


#pragma endregion  FORWARD_DECLARATIONS Entity
class Entity;
class Model;
class Entity {

#pragma region  HEADER Entity
public:
static const int ClassID = 3231396602;
static const LXType Type = LXType_Entity;
static constexpr const char* ClassName = "Entity";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const bool& GetVisible() { return m_Visible; }; 
void SetVisible(bool value) { m_Visible = value; }; 
const glm::mat4& GetModelToWorldMatrix() { return m_ModelToWorldMatrix; }; 
void SetModelToWorldMatrix(glm::mat4 value) { m_ModelToWorldMatrix = value; }; 
const glm::vec3& GetPosition() { return m_Position; }; 
void SetPosition(glm::vec3 value) { SetPositionCallback(value); }; 
void SetPositionCallback(const glm::vec3& value);
void AddTo_Position(glm::vec3 value) { m_Position += value; };
const float& GetRotationAngle() { return m_RotationAngle; }; 
void SetRotationAngle(float value) { SetRotationAngleCallback(value); }; 
void SetRotationAngleCallback(const float& value);
const glm::vec3& GetRotationAxis() { return m_RotationAxis; }; 
void SetRotationAxis(glm::vec3 value) { SetRotationAxisCallback(value); }; 
void SetRotationAxisCallback(const glm::vec3& value);
const glm::vec3& GetScale() { return m_Scale; }; 
void SetScale(glm::vec3 value) { SetScaleCallback(value); }; 
void SetScaleCallback(const glm::vec3& value);
const bool& GetHasMoved() { return m_HasMoved; }; 
void SetHasMoved(bool value) { SetHasMovedCallback(value); }; 
void SetHasMovedCallback(const bool& value);
const float& GetPickingID() { return m_PickingID; }; 
void SetPickingID(float value) { m_PickingID = value; }; 
Model*& GetModel() { return m_Model; }; 
void SetModel(Model* value) { SetModelCallback(value); }; 
void SetModelCallback(Model* value);
const bool& GetIsLight() { return m_IsLight; }; 
void SetIsLight(bool value) { m_IsLight = value; }; 
std::vector<Component*>& GetComponents() { return m_Components; }; 
void SetComponents(std::vector<Component*> value) { m_Components = value; }; 
void AddTo_Components(Component* value) { m_Components.push_back(value); };
private:
int m_ObjectID;
std::string m_Name;
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
static const int g_PropertyCount = 13;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_EntityPIDX
{
PIDX_ObjectID,
PIDX_Name,
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
bool Serialize(bool writing);
void PostSerialization(bool writing, bool success);

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
