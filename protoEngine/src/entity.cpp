#include "entity.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

using namespace glm;

#define ENTITY_SIZE 0.0001f
#define POWER_MAG ENTITY_SIZE * 10.f
#define FRICTION_COEFF 0.5f

#define ENTITY_LINE_N 5 // 3 lines for vehicle, 2 lines for local space debug representation

Entity::Entity() :  mass(1.f), maxThrust(50.f), maxForwardSpeed(50.f), maxBackwardSpeed(-10.f),
                       angle(0.f), turning(0), desiredSpeed(0.f) {

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

void Entity::Update(double dt) {
    // euler integration
    position += velocity * (float)dt;
    velocity += acceleration * (float)dt;


    if(turning) {
        angle += dt * 100.f * turning;
    }

    forwardVector = normalize(vec3(cosf(glm::radians(angle)), sinf(glm::radians(angle)), 0));
    rightVector = normalize(vec3(cosf(glm::radians(angle-90.f)), sinf(glm::radians(angle-90.f)), 0.f));

    vec3 thrust_force(0.f);
    vec3 friction_force(0.f);

    float curr_speed = GetForwardSpeed();

    // Forward thrust power
    if(desiredSpeed != 0.f) {
        thrust_force = (desiredSpeed > curr_speed ? 1 : -1) * maxThrust * forwardVector * ENTITY_SIZE;;
    }

    // Ground friction & lateral force cancellation
    // TODO : Different ground characteristics
    if(length(velocity) > 0.f) {
        // apply friction
        vec3 lateral_velocity = GetLateralVelocity();
        vec3 forward_velocity = GetForwardVelocity();
        float friction_mag = -2.f * 1000.f* POWER_MAG * length(forward_velocity);
        friction_force = friction_mag * normalize(forward_velocity);

        // apply lateral impulse to remove lateral movement
        velocity += -lateral_velocity * mass;
    }

//    std::cout << desiredSpeed << "   " << curr_speed << std::endl;

    // a = \sum(F)/m
    acceleration = (thrust_force + friction_force) / mass;
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

    glEnableVertexArrayAttrib(pPipelineEntities->glidVao, 0);
    glVertexArrayVertexBuffer(pPipelineEntities->glidVao, 0, glidEntitiesPositions, 0, sizeof(vec3));
    glVertexArrayAttribFormat(pPipelineEntities->glidVao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(pPipelineEntities->glidVao, 1);
    glVertexArrayVertexBuffer(pPipelineEntities->glidVao, 1, glidEntitiesColors, 0, sizeof(vec3));
    glVertexArrayAttribFormat(pPipelineEntities->glidVao, 1, 3, GL_FLOAT, GL_FALSE, 0);

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
    entityPositions.resize(dataSize * ENTITY_LINE_N);
    entityColors.resize(dataSize * ENTITY_LINE_N);

    glDeleteBuffers(1, &glidEntitiesPositions);
    glCreateBuffers(1, &glidEntitiesPositions);
    glNamedBufferStorage(glidEntitiesPositions, entityPositions.size() * sizeof(vec3),
                         NULL,
                         GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

    glDeleteBuffers(1, &glidEntitiesColors);
    glCreateBuffers(1, &glidEntitiesColors);
    glNamedBufferStorage(glidEntitiesColors, entityColors.size() * sizeof(vec3),
                         NULL,
                         GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

    array_modification = true;
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
    if(array_modification) {
        glNamedBufferSubData(glidEntitiesPositions, 0, (entities.size() * ENTITY_LINE_N * 2) * sizeof(vec3), entityPositions.data());
        glNamedBufferSubData(glidEntitiesColors, 0, (entities.size() * ENTITY_LINE_N * 2) * sizeof(vec3), entityColors.data());
    }

    pPipelineEntities->usePipeline();
    glProgramUniformMatrix4fv(pPipelineEntities->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineEntities->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(viewMatrix));
    glDrawArrays(GL_LINES, 0, entities.size() * ENTITY_LINE_N * 2);
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
