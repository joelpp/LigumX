#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "program_pipeline.h"
#include <iostream>
//#include "glfw\include\GLFW\glfw3.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#pragma region  FORWARD_DECLARATIONS Entity
#include "property.h"
class Model;

#pragma endregion  FORWARD_DECLARATIONS Entity
class Entity;
class Model;

class Component
{
	Entity* GetParentEntity();

	Entity* m_ParentEntity;


};

class Entity {

#pragma region  HEADER Entity
static const int ClassID = 3231396602;
public:
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const glm::vec3& GetPosition() { return m_Position; }; 
void SetPosition(glm::vec3 value) { m_Position = value; }; 
const float& GetRotationAngle() { return m_RotationAngle; }; 
void SetRotationAngle(float value) { m_RotationAngle = value; }; 
const glm::vec3& GetRotationAxis() { return m_RotationAxis; }; 
void SetRotationAxis(glm::vec3 value) { m_RotationAxis = value; }; 
const glm::vec3& GetScale() { return m_Scale; }; 
void SetScale(glm::vec3 value) { m_Scale = value; }; 
const float& GetPickingID() { return m_PickingID; }; 
void SetPickingID(float value) { m_PickingID = value; }; 
Model* GetModel() { return m_Model; }; 
void SetModel(Model* value) { m_Model = value; }; 
const bool& GetIsLight() { return m_IsLight; }; 
void SetIsLight(bool value) { m_IsLight = value; }; 
private:
std::string m_Name;
glm::vec3 m_Position;
float m_RotationAngle;
glm::vec3 m_RotationAxis;
glm::vec3 m_Scale;
float m_PickingID;
Model* m_Model;
bool m_IsLight;
public:
static const int g_EntityPropertyCount = 8;
static const ClassPropertyData g_Properties[g_EntityPropertyCount];


#pragma endregion  HEADER Entity
friend class EntityManager;
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


    float GetForwardSpeed() const;
    glm::vec3 GetForwardVelocity() const;
    glm::vec3 GetLateralVelocity() const;

	std::vector<Component*> m_Components;

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

	glm::mat4x4 m_ModelToWorldMatrix;


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

// ####################################

class EntityController {
public:
    EntityController(Entity *e) : entity(e) {}
    virtual ~EntityController() {}

    virtual void Update() {}
    virtual void OnKey(int key, int action) {}

protected:
    Entity *entity;
};

class PlayerController : public EntityController {
public:
    PlayerController(Entity *e = NULL) : EntityController(e) {}

    void Update();
    void OnKey(int key, int action);
};

class AIController : public EntityController {
public:
    AIController(Entity *e = NULL) : EntityController(e) {}

    void Update();
};

// ####################################

class EntityManager {
public:
    EntityManager();

    bool Init();
    void KeyCallback(int key, int action);
    void Update(double dt);
    void Render(const glm::mat4 &viewMatrix);

    void AddEntity(const Entity &e);

private:
    std::vector<Entity> entities;

    PlayerController playerController;
    std::vector<AIController> aiControllers;

    // internal rendering stuff
    ProgramPipeline* pPipelineEntities;
    bool array_modification;
    size_t dataSize;

    std::vector<glm::vec3> entityPositions;
    GLuint glidEntitiesPositions;

    std::vector<glm::vec3> entityColors;
    GLuint glidEntitiesColors;

    void makeVBO();
};


#endif // ENTITY_H
