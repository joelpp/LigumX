#include "stdafx.h"

#include "glm/glm.hpp"

#pragma region  CLASS_SOURCE Entity
#include "Entity.h"
#include <cstddef>
const ClassPropertyData Entity::g_Properties[] = 
{
{ "Name", offsetof(Entity, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "Position", offsetof(Entity, m_Position), 0, LXType_glmvec3, false, LXType_None, 0, 0, 0, }, 
{ "RotationAngle", offsetof(Entity, m_RotationAngle), 0, LXType_float, false, LXType_None, 0, 0, 0, }, 
{ "RotationAxis", offsetof(Entity, m_RotationAxis), 0, LXType_glmvec3, false, LXType_None, 0, 0, 0, }, 
{ "Scale", offsetof(Entity, m_Scale), 0, LXType_glmvec3, false, LXType_None, 0, 0, 0, }, 
{ "PickingID", offsetof(Entity, m_PickingID), 0, LXType_float, false, LXType_None, PropertyFlags_Hidden, 0, 0, }, 
{ "Model", offsetof(Entity, m_Model), 0, LXType_Model, true, LXType_None, 0, 0, 0, }, 
{ "IsLight", offsetof(Entity, m_IsLight), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
};

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

	// todo : check out what adrien did for cars
	// , but for now it sleeps safely in source control
}

// #############################################

EntityManager::EntityManager() : array_modification(false) {

}

bool EntityManager::Init() {
    const char* vertexShaderSource = " \
        #version 410 core\n \
        layout(location=0) in vec2 pos;\n \
        layout(location=1) in vec3 color;\n \
        uniform mat4 vpMat;\n \
        out gl_PerVertex {\n \
            vec4 gl_Position;\n \
        };\n \
        out vec3 vColor;\n \
        void main() {\n \
            gl_Position = vpMat * vec4(pos, 0, 1);\n \
            vColor = color;\n \
        }";

    const char* fragmentShaderSource = " \
        #version 410 core\n \
        in vec3 vColor;\n \
        out vec3 color;\n \
        void main() {\n \
            color = vColor;\n \
        }";

    ProgramPipeline::ShaderProgram* pVertexShader =
            new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER,
            vertexShaderSource, true);

    ProgramPipeline::ShaderProgram* pFragmentShader =
            new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
            fragmentShaderSource, true);

    pPipelineEntities = new ProgramPipeline();
    pPipelineEntities->useVertexShader(pVertexShader);
    pPipelineEntities->useFragmentShader(pFragmentShader);

    // Fixed starting size for data arrays
    dataSize = 100;


    makeVBO();

    // glEnableVertexArrayAttrib(pPipelineEntities->glidVao, 0);
    // glVertexArrayVertexBuffer(pPipelineEntities->glidVao, 0, glidEntitiesPositions, 0, sizeof(vec3));
    // glVertexArrayAttribFormat(pPipelineEntities->glidVao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    // glEnableVertexArrayAttrib(pPipelineEntities->glidVao, 1);
    // glVertexArrayVertexBuffer(pPipelineEntities->glidVao, 1, glidEntitiesColors, 0, sizeof(vec3));
    // glVertexArrayAttribFormat(pPipelineEntities->glidVao, 1, 3, GL_FLOAT, GL_FALSE, 0);

    Entity e;
    e.position = vec3(-5 * ENTITY_SIZE,0,1);
    e.color = vec3(1,0.8,0);
    e.type = Entity::CONTROLLER_PLAYER;
    e.mass = 1;
    AddEntity(e);

    return true;
}

void EntityManager::AddEntity(const Entity &entity) {
    Entity e(entity);

    e.forwardVector = normalize(vec3(cosf(glm::radians(e.angle)), sinf(glm::radians(e.angle)), 0));
    e.rightVector = normalize(vec3(cosf(glm::radians(e.angle-90.f)), sinf(glm::radians(e.angle-90.f)), 0));
    e.entityIndex = entities.size();

    // resize arrays/vbos if we get too much entities
    if(entities.size() > dataSize) {
        dataSize *= 2;
        makeVBO();
    }


    entityPositions[(0+e.entityIndex)*2] = e.position - 0.5f * ENTITY_SIZE * e.rightVector;
    entityPositions[(0+e.entityIndex)*2+1] = e.position + 0.5f * ENTITY_SIZE * e.rightVector;
    entityPositions[(1+e.entityIndex)*2] = e.position - 0.5f * ENTITY_SIZE * e.rightVector;
    entityPositions[(1+e.entityIndex)*2+1] = e.position + 1.5f * ENTITY_SIZE * e.forwardVector;
    entityPositions[(2+e.entityIndex)*2] = e.position + 0.5f * ENTITY_SIZE * e.rightVector;;
    entityPositions[(2+e.entityIndex)*2+1] = e.position + 1.5f * ENTITY_SIZE * e.forwardVector;
    entityPositions[(3+e.entityIndex)*2] = e.position;
    entityPositions[(3+e.entityIndex)*2+1] = e.position + ENTITY_SIZE * e.rightVector;
    entityPositions[(4+e.entityIndex)*2] = e.position;
    entityPositions[(4+e.entityIndex)*2+1] = e.position + 0.1f * ENTITY_SIZE * e.forwardVector;

    entityColors[(0+e.entityIndex)*2] = e.color;
    entityColors[(0+e.entityIndex)*2+1] = e.color;
    entityColors[(1+e.entityIndex)*2] = e.color;
    entityColors[(1+e.entityIndex)*2+1] = e.color;
    entityColors[(2+e.entityIndex)*2] = e.color;
    entityColors[(2+e.entityIndex)*2+1] = e.color;
    entityColors[(3+e.entityIndex)*2] = vec3(1,0,0);
    entityColors[(3+e.entityIndex)*2+1] = vec3(1,0,0);
    entityColors[(4+e.entityIndex)*2] = vec3(0,1,0);
    entityColors[(4+e.entityIndex)*2+1] = vec3(0,1,0);
    array_modification = true;

    entities.push_back(e);


    // Controller type
    if(e.type == Entity::CONTROLLER_PLAYER) {
        playerController = PlayerController(&entities[entities.size()-1]);
    } else {
        aiControllers.push_back(AIController(&entities[entities.size()-1]));
    }
}

void EntityManager::makeVBO() {
    // entityPositions.resize(dataSize * ENTITY_LINE_N);
    // entityColors.resize(dataSize * ENTITY_LINE_N);

    // glDeleteBuffers(1, &glidEntitiesPositions);
    // glCreateBuffers(1, &glidEntitiesPositions);
    // glNamedBufferStorage(glidEntitiesPositions, entityPositions.size() * sizeof(vec3),
    //                      NULL,
    //                      GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

    // glDeleteBuffers(1, &glidEntitiesColors);
    // glCreateBuffers(1, &glidEntitiesColors);
    // glNamedBufferStorage(glidEntitiesColors, entityColors.size() * sizeof(vec3),
    //                      NULL,
    //                      GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

    // array_modification = true;
}

void EntityManager::KeyCallback(int key, int action) {
    // playerController.OnKey(key, action);
    // for(size_t i = 0; i < aiControllers.size(); ++i)
    //     aiControllers[i].OnKey(key, action);
}

void EntityManager::Update(double dt) {
    for(size_t i = 0; i < entities.size(); ++i) {
        Entity &e = entities[i];

        e.Update(dt);

        // TODO : Find when an entity's rendering data shouldnt be updated
        entityPositions[(0+e.entityIndex)*2] = e.position - 0.5f * ENTITY_SIZE * e.rightVector;
        entityPositions[(0+e.entityIndex)*2+1] = e.position + 0.5f * ENTITY_SIZE * e.rightVector;
        entityPositions[(1+e.entityIndex)*2] = e.position - 0.5f * ENTITY_SIZE * e.rightVector;
        entityPositions[(1+e.entityIndex)*2+1] = e.position + 1.5f * ENTITY_SIZE * e.forwardVector;
        entityPositions[(2+e.entityIndex)*2] = e.position + 0.5f * ENTITY_SIZE * e.rightVector;
        entityPositions[(2+e.entityIndex)*2+1] = e.position + 1.5f * ENTITY_SIZE * e.forwardVector;
        entityPositions[(3+e.entityIndex)*2] = e.position;
        entityPositions[(3+e.entityIndex)*2+1] = e.position + ENTITY_SIZE * e.rightVector;
        entityPositions[(4+e.entityIndex)*2] = e.position;
        entityPositions[(4+e.entityIndex)*2+1] = e.position + length(e.velocity) * e.forwardVector;
        array_modification = true;
    }
}

void EntityManager::Render(const mat4 &viewMatrix) {
    // if(array_modification) {
    //     glNamedBufferSubData(glidEntitiesPositions, 0, (entities.size() * ENTITY_LINE_N * 2) * sizeof(vec3), entityPositions.data());
    //     glNamedBufferSubData(glidEntitiesColors, 0, (entities.size() * ENTITY_LINE_N * 2) * sizeof(vec3), entityColors.data());
    // }

    // pPipelineEntities->usePipeline();
    // glProgramUniformMatrix4fv(pPipelineEntities->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineEntities->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(viewMatrix));
    // glDrawArrays(GL_LINES, 0, entities.size() * ENTITY_LINE_N * 2);
}



// #####################################################
void PlayerController::Update() {
}

void PlayerController::OnKey(int key, int action) {
    assert(entity);
    if(action == GLFW_PRESS){
        if(key == GLFW_KEY_KP_4) { entity->turning = 1; }
        if(key == GLFW_KEY_KP_6) { entity->turning = -1;}

        if(key == GLFW_KEY_KP_8) { entity->desiredSpeed = ENTITY_SIZE * entity->maxForwardSpeed;}
        if(key == GLFW_KEY_KP_5) { entity->desiredSpeed = ENTITY_SIZE * entity->maxBackwardSpeed;}

        if(key == GLFW_KEY_F5) {
            entity->position = vec3(-5 * ENTITY_SIZE,0,1);
            entity->acceleration = entity->velocity = vec3(0,0,0);
            entity->angle = entity->desiredSpeed = 0.f;
            entity->turning = 0;
        }
    }

    if(action == GLFW_RELEASE) {
        if((key == GLFW_KEY_KP_4 && entity->turning == 1) ||
                (key == GLFW_KEY_KP_6 && entity->turning == -1)) {
            entity->turning = 0;
        }
        if((key == GLFW_KEY_KP_8 && entity->desiredSpeed > 0.f) ||
                (key == GLFW_KEY_KP_5 && entity->desiredSpeed < 0.f)) {
            entity->desiredSpeed = 0.f;
        }
    }
}

void AIController::Update() {
}
