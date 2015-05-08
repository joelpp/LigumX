#include "entity.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#define printvec3(v) std::cout << v.x << " " << v.y << " " << v.z << std::endl
using namespace glm;

#define ENTITY_SIZE 0.0001f
#define POWER_MAG ENTITY_SIZE * 10.f
#define FRICTION_COEFF 0.5f
#define DEFAULT_ENTITY_TYPE CarEntityType

class CarDef : public EntityType {
public:
    CarDef()  {
        segmentCount = 22;
        mass = 1500.f;
        maxThrust = 50.f;
        resistanceDrag = 0.4257f;
        resistanceRolling = 30.f * resistanceDrag;
        forwardGears = 6;
        backwardGears = 1;
        differentialRatio = 3.42f;
        wheelRadius = 0.34f;

        gearRatios[0] = 1.f/2.66f;
        gearRatios[1] = 1.f/1.78f;
        gearRatios[2] = 1.f/1.30f;
        gearRatios[3] = 1.f/1.00f;
        gearRatios[4] = 1.f/0.74f;
        gearRatios[5] = 1.f/0.50f;

        wheelWidth = 0.075f;
        wheelHeight = 0.2f;
    }

    void MakeDataPositions(glm::vec3 *target, const Body &b) {
        const vec3 right = b.right;
        const vec3 fwd = b.forward;
        const vec3 pos = b.position;

        target[0*2] = pos - 0.5f * ENTITY_SIZE * right;
        target[0*2+1] = pos + 0.5f * ENTITY_SIZE * right;
        target[1*2] = pos - 0.5f * ENTITY_SIZE * right;
        target[1*2+1] = pos + 1.5f * ENTITY_SIZE * fwd - 0.2f * ENTITY_SIZE * right;
        target[2*2] = pos + 0.5f * ENTITY_SIZE * right;
        target[2*2+1] = pos + 1.5f * ENTITY_SIZE * fwd + 0.2f * ENTITY_SIZE * right;
        target[3*2] = pos + 1.5f * ENTITY_SIZE * fwd - 0.2f * ENTITY_SIZE * right;
        target[3*2+1] = pos + 1.5f * ENTITY_SIZE * fwd + 0.2f * ENTITY_SIZE * right;

        // Create wheels
        MakeWheelAt(&target[4*2], pos + 1.5f * ENTITY_SIZE * fwd - 0.5f * ENTITY_SIZE * right, ENTITY_SIZE * fwd, ENTITY_SIZE * right);
        MakeWheelAt(&target[8*2], pos + 1.5f * ENTITY_SIZE * fwd + 0.5f * ENTITY_SIZE * right, ENTITY_SIZE * fwd, ENTITY_SIZE * right);
        MakeWheelAt(&target[12*2], pos - 0.6f * ENTITY_SIZE * right, ENTITY_SIZE * fwd, ENTITY_SIZE * right);
        MakeWheelAt(&target[16*2], pos + 0.6f * ENTITY_SIZE * right, ENTITY_SIZE * fwd, ENTITY_SIZE * right);

        // debug visuals
        target[(segmentCount-2)*2] = pos;
        target[(segmentCount-2)*2+1] = pos + ENTITY_SIZE * right;
        target[(segmentCount-1)*2] = pos;
        target[(segmentCount-1)*2+1] = pos + /*ENTITY_SIZE **/ b.GetForwardVelocity();
    }

    void MakeDataColors(glm::vec3 *target, const Entity &e) {
        for(size_t i = 0; i < segmentCount-2; ++i) {
            target[2*i] = e.color;
            target[2*i+1] = e.color;
        }

        // color of debug visuals
        target[(segmentCount-2)*2] = vec3(1,0,0);
        target[(segmentCount-2)*2+1] = vec3(1,0,0);
        target[(segmentCount-1)*2] = vec3(0,1,0);
        target[(segmentCount-1)*2+1] = vec3(0,1,0);
    }

    void UpdateLogic(float dt, Body &b, int control_state) {
        float engine_force = 0.f;
        float air_resistance = 0.f;
        float rolling_resistance = 0.f;

        vec3 velocity_vector = b.GetForwardVelocity();
        float curr_speed = length(velocity_vector);
        vec3 forward_normal = b.ToLocal(vec3(0, 1, 0));

        // Forward thrust power
        if(control_state & Entity::CS_FWD) {
            if(!b.speedMult) {
                b.speedMult = 1;
                std::cout << b.speedMult << std::endl;
                b.rpm = 10;
            }
            engine_force = maxThrust * ENTITY_SIZE;
        } else if(control_state & Entity::CS_BWD) {
            if(!b.speedMult) {
                b.speedMult = -1;
                std::cout << b.speedMult << std::endl;
                b.rpm = 10;
            }
            engine_force = -maxThrust * ENTITY_SIZE;
        }

        b.wheel_acceleration = engine_force * gearRatios[b.speedMult] * differentialRatio;

        // integrate
        b.wheel_velocity += b.wheel_acceleration * (float)dt;


        // Update engine RPM and gear level if necessary
        b.rpm = b.wheel_velocity * gearRatios[b.speedMult] * differentialRatio * 60 / (2.f * M_PI);

        if(b.speedMult < 0) {
            if(b.rpm < 100.f) {
                b.speedMult = 0;
                std::cout << b.speedMult << std::endl;
            }
            // no else, only 1 gear in reverse (TODO : more reverse gear, is that even a thing ??)
        }
        else if(b.speedMult > 0) {
            if(b.rpm < 100.f) {
                --b.speedMult;
                std::cout << b.speedMult << std::endl;
            } else if(b.rpm > gearRatios[b.speedMult] * 3000) {
                ++b.speedMult;
                std::cout << b.speedMult << std::endl;
            }
        }


        float wheel_force = b.wheel_velocity / wheelRadius;

        // Ground friction & lateral force cancellation
        // TODO : Different ground characteristics
        if(curr_speed > 0.f) {
            // F_drag = - C_drag * vel * |vel|
            air_resistance = -resistanceDrag * curr_speed;
            // F_rolling = - C_rolling * vel
            rolling_resistance = -resistanceRolling;
        }

        // a = \sum(F)/m
        b.acceleration += (wheel_force * forward_normal + (air_resistance + rolling_resistance) * b.velocity) / mass;
    }

private:
    void MakeWheelAt(vec3 *dataPtr, vec3 pos, vec3 fwd, vec3 right) {
        dataPtr[0*2] = pos - wheelWidth * right + wheelHeight * fwd;
        dataPtr[0*2+1] = pos + wheelWidth * right + wheelHeight * fwd;
        dataPtr[1*2] = pos - wheelWidth * right + wheelHeight * fwd;
        dataPtr[1*2+1] = pos - wheelWidth * right - wheelHeight * fwd;
        dataPtr[2*2] = pos - wheelWidth * right - wheelHeight * fwd;
        dataPtr[2*2+1] = pos + wheelWidth * right - wheelHeight * fwd;
        dataPtr[3*2] = pos + wheelWidth * right + wheelHeight * fwd;
        dataPtr[3*2+1] = pos + wheelWidth * right - wheelHeight * fwd;
    }

    float wheelWidth;
    float wheelHeight;
};

CarDef CarEntityType;



// ###################################################

Body::Body() : position(0.f), mass(1), speedMult(0), rpm(0), wheel_velocity(0.f), wheel_acceleration(0.f), awake(true) {
    ResetLocalSpace();
}

void Body::Update(double dt, int control_state) {
    acceleration = vec3(0.f);

//    float d_angle = dt * 100.f * turning;
//    if(turning) {
//        angle += d_angle;
//    }

//    forward = normalize(vec3(cosf(glm::radians(angle)), sinf(glm::radians(angle)), 0));
//    right = normalize(vec3(cosf(glm::radians(angle-90.f)), sinf(glm::radians(angle-90.f)), 0.f));

//    printvec3(velocity_vector);
//    std::cout << curr_speed << std::endl;

    type->UpdateLogic((float)dt, *this, control_state);

    // euler integration
    velocity += acceleration * (float)dt;
    position += velocity * (float)dt;
}

void Body::GenerateLocalSpace() {
    right = AngleToVector(radians(angle));
    forward = AngleToVector(radians(angle+90.f));
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

Entity::Entity() : color(1), controller(CONTROLLER_AI), controlState(0), type(&DEFAULT_ENTITY_TYPE)  {

}



void Entity::Update(double dt) {

}

// #############################################

EntityDesc::EntityDesc() : position(0), angle(0.f), color(1.f), controller(Entity::CONTROLLER_AI), type(&DEFAULT_ENTITY_TYPE) {

}

EntityManager::EntityManager() : array_modification(false), dataSize(0), filledSize(0) {

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
    dataSize = 256;
    srand(time(NULL));


    makeVBO();

    glEnableVertexArrayAttrib(pPipelineEntities->glidVao, 0);
    glVertexArrayVertexBuffer(pPipelineEntities->glidVao, 0, glidEntitiesPositions, 0, sizeof(vec3));
    glVertexArrayAttribFormat(pPipelineEntities->glidVao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(pPipelineEntities->glidVao, 1);
    glVertexArrayVertexBuffer(pPipelineEntities->glidVao, 1, glidEntitiesColors, 0, sizeof(vec3));
    glVertexArrayAttribFormat(pPipelineEntities->glidVao, 1, 3, GL_FLOAT, GL_FALSE, 0);

    EntityDesc e;
    e.position = vec3(-5 * ENTITY_SIZE,0,1);
    e.angle = 0.f;
    e.color = vec3(1,0.8,0);
    e.type = &CarEntityType;
    e.controller = Entity::CONTROLLER_PLAYER;
    AddEntity(e);

//    for(size_t i = 0; i < 20; ++i) {
//        Entity e;
//        e.position = vec3((rand01() * 40 - 20)  * ENTITY_SIZE, (rand01() * 40 - 20)  * ENTITY_SIZE, 1);
//        e.angle = rand01() * 360;
//        e.color = vec3(rand01() * 0.7 + 0.3, rand01() * 0.7 + 0.3, rand01() * 0.7 + 0.3);
//        e.type = Entity::CONTROLLER_AI;
//        e.mass = 1;
//        AddEntity(e);
//    }

    return true;
}

void EntityManager::AddEntity(const EntityDesc &entity_desc) {
    size_t ei = entities.size();

    size_t dataIndex = filledSize;
    filledSize += entity_desc.type->segmentCount * 2; // 2 vertices per segment

    // Make sure we have enough space for secondary arrays
    if(filledSize >= dataSize) {
        while(filledSize >= dataSize) {
            dataSize *= 2;
            makeVBO();
        }
    }

    // add entity-body pair to storage
    entities.push_back(Entity());
    bodies.push_back(Body());

    // Initialize the pair
    Entity &e = GetEntity(ei);
    Body &b = GetBody(ei);

    e.entityIndex = ei;
    e.color = entity_desc.color;
    e.type = entity_desc.type;
    e.controller = entity_desc.controller;
    e.dataIndex = dataIndex;

    b.angle = entity_desc.angle;
    b.position = entity_desc.position;
    b.type = entity_desc.type;
    b.mass = b.type->mass;
    b.GenerateLocalSpace();

    // Generate mesh
    e.type->MakeDataPositions(&entityPositions[e.dataIndex], b);
    e.type->MakeDataColors(&entityColors[e.dataIndex], e);

    // Controller type
    if(e.controller == Entity::CONTROLLER_PLAYER) {
        playerController = PlayerController(this, e.entityIndex);
    } else {
        aiControllers.push_back(AIController(this, e.entityIndex));
    }
}

void EntityManager::makeVBO() {
    entityPositions.resize(dataSize);
    entityColors.resize(dataSize);

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
        Body &b = bodies[i];

        e.Update(dt);
        b.Update(dt, e.controlState);

        // TODO : Find when an entity's rendering data shouldnt be updated
        e.type->MakeDataPositions(&entityPositions[e.dataIndex], b);
        array_modification = true;
    }
}

void EntityManager::Render(const mat4 &viewMatrix) {
    if(array_modification) {
        glNamedBufferSubData(glidEntitiesPositions, 0, filledSize * sizeof(vec3), entityPositions.data());
        glNamedBufferSubData(glidEntitiesColors, 0, filledSize * sizeof(vec3), entityColors.data());
    }

    pPipelineEntities->usePipeline();
    glProgramUniformMatrix4fv(pPipelineEntities->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineEntities->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(viewMatrix));
    glDrawArrays(GL_LINES, 0, filledSize);
}



// #####################################################
void PlayerController::Update() {
}

void PlayerController::OnKey(int key, int action) {
    assert(entityIndex >= 0);
    Entity &e = entityManager->GetEntity(entityIndex);
    Body &b = entityManager->GetBody(entityIndex);

    if(action == GLFW_PRESS){
        if(key == GLFW_KEY_KP_4) { e.controlState |= Entity::CS_LEFT; }
        if(key == GLFW_KEY_KP_6) { e.controlState |= Entity::CS_RIGHT; }

        if(key == GLFW_KEY_KP_8) { e.controlState |= Entity::CS_FWD; }
        if(key == GLFW_KEY_KP_5) { e.controlState |= Entity::CS_BWD; }

        if(key == GLFW_KEY_F5) {
//            e.position = vec3(-5 * ENTITY_SIZE,0,1);
//            e.acceleration = e.velocity = vec3(0,0,0);
//            e.angle = e.desiredSpeed = 0.f;
//            e.turning = 0;
        }
    }

    if(action == GLFW_RELEASE) {
        if(key == GLFW_KEY_KP_4) { e.controlState &= ~Entity::CS_LEFT; }
        if(key == GLFW_KEY_KP_6) { e.controlState &= ~Entity::CS_RIGHT; }

        if(key == GLFW_KEY_KP_8) { e.controlState &= ~Entity::CS_FWD; }
        if(key == GLFW_KEY_KP_5) { e.controlState &= ~Entity::CS_BWD; }
    }
}

void AIController::Update() {
}
