#include "entity.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

using namespace glm;

#define ENTITY_SIZE 0.0001f
#define POWER_MAG ENTITY_SIZE * 10.f
#define FRICTION_COEFF 0.5f

class BasicVehicleDef {
public:
    static void FillPositions(vec3 *dataPtr, const Entity &e) {
        const Body &b = e.GetBody();
        const vec3 right = b.right;
        const vec3 fwd = b.forward;
        const vec3 pos = b.position;

        dataPtr[0*2] = pos - 0.5f * ENTITY_SIZE * right;
        dataPtr[0*2+1] = pos + 0.5f * ENTITY_SIZE * right;
        dataPtr[1*2] = pos - 0.5f * ENTITY_SIZE * right;
        dataPtr[1*2+1] = pos + 1.5f * ENTITY_SIZE * fwd;
        dataPtr[2*2] = pos + 0.5f * ENTITY_SIZE * right;;
        dataPtr[2*2+1] = pos + 1.5f * ENTITY_SIZE * fwd;

        // Create wheels
        MakeWheelAt(&dataPtr[3*2], pos + 1.5f * ENTITY_SIZE * fwd - 0.5f * ENTITY_SIZE * right, ENTITY_SIZE * fwd, ENTITY_SIZE * right);
        MakeWheelAt(&dataPtr[7*2], pos + 1.5f * ENTITY_SIZE * fwd + 0.5f * ENTITY_SIZE * right, ENTITY_SIZE * fwd, ENTITY_SIZE * right);
        MakeWheelAt(&dataPtr[11*2], pos - 0.6f * ENTITY_SIZE * right, ENTITY_SIZE * fwd, ENTITY_SIZE * right);
        MakeWheelAt(&dataPtr[15*2], pos + 0.6f * ENTITY_SIZE * right, ENTITY_SIZE * fwd, ENTITY_SIZE * right);

        // debug visuals
        dataPtr[(lineCount-2)*2] = pos;
        dataPtr[(lineCount-2)*2+1] = pos + ENTITY_SIZE * right;
        dataPtr[(lineCount-1)*2] = pos;
        dataPtr[(lineCount-1)*2+1] = pos + b.GetForwardVelocity() / 6.f;
    }

    static void FillColors(vec3 *dataPtr, const Entity &e) {
        for(size_t i = 0; i < lineCount-2; ++i) {
            dataPtr[2*i] = e.color;
            dataPtr[2*i+1] = e.color;
        }

        // color of debug visuals
        dataPtr[(lineCount-2)*2] = vec3(1,0,0);
        dataPtr[(lineCount-2)*2+1] = vec3(1,0,0);
        dataPtr[(lineCount-1)*2] = vec3(0,1,0);
        dataPtr[(lineCount-1)*2+1] = vec3(0,1,0);
    }

    static size_t LineCount() {
        return lineCount;
    }

private:
    static void MakeWheelAt(vec3 *dataPtr, vec3 pos, vec3 fwd, vec3 right) {
        dataPtr[0*2] = pos - wheelWidth * right + wheelHeight * fwd;
        dataPtr[0*2+1] = pos + wheelWidth * right + wheelHeight * fwd;
        dataPtr[1*2] = pos - wheelWidth * right + wheelHeight * fwd;
        dataPtr[1*2+1] = pos - wheelWidth * right - wheelHeight * fwd;
        dataPtr[2*2] = pos - wheelWidth * right - wheelHeight * fwd;
        dataPtr[2*2+1] = pos + wheelWidth * right - wheelHeight * fwd;
        dataPtr[3*2] = pos + wheelWidth * right + wheelHeight * fwd;
        dataPtr[3*2+1] = pos + wheelWidth * right - wheelHeight * fwd;
    }

    static const size_t lineCount;
    static const float wheelWidth;
    static const float wheelHeight;
    // Entity features
    static const float maxThrust;                 //!< Maximum thrust acceleration
    static const float maxForwardSpeed;      //!< Maximum atteignable forward speed
    static const float maxBackwardSpeed;    //!< Maximum atteignable backward speed
};

// Definition of BasicVehicleDef static variables
const size_t BasicVehicleDef::lineCount = 21;
const float BasicVehicleDef::wheelWidth = 0.075f;
const float BasicVehicleDef::wheelHeight = 0.2f;
const float BasicVehicleDef::maxThrust = 50.f;                 //!< Maximum thrust acceleration
const float BasicVehicleDef::maxForwardSpeed = 50.f;      //!< Maximum atteignable forward speed
const float BasicVehicleDef::maxBackwardSpeed = -10.f;


// ###################################################

Body::Body() : position(0.f), mass(1), awake(true) {
    ResetLocalSpace();
}

void Body::Update(double dt) {
    // euler integration
    position += velocity * (float)dt;
    velocity += acceleration * (float)dt;


//    float d_angle = dt * 100.f * turning;
//    if(turning) {
//        angle += d_angle;
//    }

//    forward = normalize(vec3(cosf(glm::radians(angle)), sinf(glm::radians(angle)), 0));
//    right = normalize(vec3(cosf(glm::radians(angle-90.f)), sinf(glm::radians(angle-90.f)), 0.f));

    vec3 thrust_force(0.f);
    vec3 friction_force(0.f);

    float curr_speed = GetForwardSpeed();

    // Forward thrust power
    if(desiredSpeed != 0.f) {
        thrust_force = (desiredSpeed > curr_speed ? 1 : -1) * maxThrust * forward * ENTITY_SIZE;
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

void Body::ResetLocalSpace() {
    angle = 0.f;
    forward = glm::vec3(0, 1, 0);
    right = glm::vec3(1, 0, 0);
}

vec3 Body::ToLocal(vec3 global_vector) const {
    return vec3(dot(global_vector, right), dot(global_vector, forward), 0.f);
}

glm::vec3 Body::ToGlobal(glm::vec3 local_vector) const {
    return right * local_vector.x + forward * local_vector.y;
}

float Body::GetForwardSpeed() const {
    return dot(forward, velocity);
}

vec3 Body::GetForwardVelocity() const {
    return forward * dot(forward, velocity);
}

vec3 Body::GetLateralVelocity() const {
    return right * dot(right, velocity);
}

// ###################################################

Entity::Entity() :
                       angle(0.f), turning(0), desiredSpeed(0.f) {

}



void Entity::Update(double dt) {

}

// #############################################

EntityManager::EntityManager() : array_modification(false) {

}

float rand01() {
    return rand()/(float)RAND_MAX;
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
    srand(time(NULL));


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

    for(size_t i = 0; i < 20; ++i) {
        Entity e;
        e.position = vec3((rand01() * 40 - 20)  * ENTITY_SIZE, (rand01() * 40 - 20)  * ENTITY_SIZE, 1);
        e.angle = rand01() * 360;
        e.color = vec3(rand01() * 0.7 + 0.3, rand01() * 0.7 + 0.3, rand01() * 0.7 + 0.3);
        e.type = Entity::CONTROLLER_AI;
        e.mass = 1;
        AddEntity(e);
    }

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


    BasicVehicleDef::FillPositions(&entityPositions[e.entityIndex*BasicVehicleDef::LineCount()*2], e);
    BasicVehicleDef::FillColors(&entityColors[e.entityIndex*BasicVehicleDef::LineCount()*2], e);
    entities.push_back(e);


    // Controller type
    if(e.type == Entity::CONTROLLER_PLAYER) {
        playerController = PlayerController(this, e.entityIndex);
    } else {
        aiControllers.push_back(AIController(this, e.entityIndex));
    }
}

void EntityManager::makeVBO() {
    entityPositions.resize(dataSize * BasicVehicleDef::LineCount());
    entityColors.resize(dataSize * BasicVehicleDef::LineCount());

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
    playerController.OnKey(key, action);
    for(size_t i = 0; i < aiControllers.size(); ++i)
        aiControllers[i].OnKey(key, action);
}

void EntityManager::Update(double dt) {
    for(size_t i = 0; i < entities.size(); ++i) {
        Entity &e = entities[i];

        e.Update(dt);


        // TODO : Find when an entity's rendering data shouldnt be updated
        BasicVehicleDef::FillPositions(&entityPositions[e.entityIndex*BasicVehicleDef::LineCount()*2], e);
        array_modification = true;
    }
}

void EntityManager::Render(const mat4 &viewMatrix) {
    if(array_modification) {
        glNamedBufferSubData(glidEntitiesPositions, 0, (entities.size() * BasicVehicleDef::LineCount() * 2) * sizeof(vec3), entityPositions.data());
        glNamedBufferSubData(glidEntitiesColors, 0, (entities.size() * BasicVehicleDef::LineCount() * 2) * sizeof(vec3), entityColors.data());
    }

    pPipelineEntities->usePipeline();
    glProgramUniformMatrix4fv(pPipelineEntities->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineEntities->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(viewMatrix));
    glDrawArrays(GL_LINES, 0, entities.size() * BasicVehicleDef::LineCount() * 2);
}



// #####################################################
void PlayerController::Update() {
}

void PlayerController::OnKey(int key, int action) {
    assert(entityIndex >= 0);
    Entity &e = entityManager->GetEntity(entityIndex);

    if(action == GLFW_PRESS){
        if(key == GLFW_KEY_KP_4) { e.turning = 1; }
        if(key == GLFW_KEY_KP_6) { e.turning = -1;}

        if(key == GLFW_KEY_KP_8) { e.desiredSpeed = ENTITY_SIZE * e.maxForwardSpeed;}
        if(key == GLFW_KEY_KP_5) { e.desiredSpeed = ENTITY_SIZE * e.maxBackwardSpeed;}

        if(key == GLFW_KEY_F5) {
            e.position = vec3(-5 * ENTITY_SIZE,0,1);
            e.acceleration = e.velocity = vec3(0,0,0);
            e.angle = e.desiredSpeed = 0.f;
            e.turning = 0;
        }
    }

    if(action == GLFW_RELEASE) {
        if((key == GLFW_KEY_KP_4 && e.turning == 1) ||
                (key == GLFW_KEY_KP_6 && e.turning == -1)) {
            e.turning = 0;
        }
        if((key == GLFW_KEY_KP_8 && e.desiredSpeed > 0.f) ||
                (key == GLFW_KEY_KP_5 && e.desiredSpeed < 0.f)) {
            e.desiredSpeed = 0.f;
        }
    }
}

void AIController::Update() {
}
