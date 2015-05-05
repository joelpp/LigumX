#include "entity.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

using namespace glm;

#define ENTITY_SIZE 0.0001f
#define POWER_MAG ENTITY_SIZE * 10.f
#define FRICTION_COEFF 0.5f

namespace Car {
    struct Car1 {
        static constexpr float mass = 1.f;
        static constexpr float maxThrust = 35.f;
    };
/*
    void Helper::BuildBody(std::vector<DynamicBody> &dataPtr, const Entity *e) {
        // Build Car Hull
        DynamicBody hull;
        hull.position = vec3(0);
        hull.mass = Car1::mass;
        hull.maxThrust = Car1::maxThrust;
        hull.angle = 0;
        hull.forwardVector = normalize(vec3(cosf(glm::radians(hull.angle)), sinf(glm::radians(hull.angle)), 0));
        hull.rightVector = normalize(vec3(cosf(glm::radians(hull.angle-90.f)), sinf(glm::radians(hull.angle-90.f)), 0));
        dataPtr.push_back(hull);

        // Build 4 wheels
        DynamicBody wheel;
        wheel.mass = 1;
        wheel.maxThrust = Car1::maxThrust;
        wheel.angle = 0;
        wheel.forwardVector = hull.forwardVector;
        wheel.rightVector = hull.rightVector;

        // FL
        wheel.position = hull.position + hull.forwardVector * 1.5f * ENTITY_SIZE - hull.rightVector * 0.5f * ENTITY_SIZE;
        dataPtr.push_back(wheel);
        // FR
        wheel.position = hull.position + hull.forwardVector * 1.5f * ENTITY_SIZE + hull.rightVector * 0.5f * ENTITY_SIZE;
        dataPtr.push_back(wheel);
        // RL
        wheel.position = hull.position - hull.rightVector * 0.6f * ENTITY_SIZE;
        dataPtr.push_back(wheel);
        // RR
        wheel.position = hull.position + hull.rightVector * 0.6f * ENTITY_SIZE;
        dataPtr.push_back(wheel);
    }
*/
    void Helper::SetPosition(const glm::vec3 &pos, Entity *e) {
       /* DynamicBody &hull = e->GetBody();
        hull.position = pos;

        // set wheel position
        DynamicBody *wheel = Misc::GetSubpart(&hull, Misc::CAR_WHEEL_FL);
        wheel->position = hull.position + hull.forwardVector * 1.5f * ENTITY_SIZE - hull.rightVector * 0.5f * ENTITY_SIZE;
        wheel = Misc::GetSubpart(&hull, Misc::CAR_WHEEL_FR);
        wheel->position = hull.position + hull.forwardVector * 1.5f * ENTITY_SIZE + hull.rightVector * 0.5f * ENTITY_SIZE;
        wheel = Misc::GetSubpart(&hull, Misc::CAR_WHEEL_RL);
        wheel->position = hull.position - hull.rightVector * 0.6f * ENTITY_SIZE;
        wheel = Misc::GetSubpart(&hull, Misc::CAR_WHEEL_RR);
        wheel->position = hull.position + hull.rightVector * 0.6f * ENTITY_SIZE;
        */
    }

    void Helper::SetAngle(float angle, Entity *e) {
    /*
        DynamicBody &hull = e->GetBody();
        hull.angle = angle;
        hull.forwardVector = normalize(vec3(cosf(glm::radians(hull.angle)), sinf(glm::radians(hull.angle)), 0));
        hull.rightVector = normalize(vec3(cosf(glm::radians(hull.angle-90.f)), sinf(glm::radians(hull.angle-90.f)), 0));

        // Recompute position of wheels from new angle
        SetPosition(hull.position, e);
        */
    }

    void Helper::FillPositions(glm::vec3 *dataPtr, const Entity *e) {
        // Display Hull
        const b2Body *body = e->GetBody();
        const float angle = body->GetAngle();
        const vec3 right = normalize(vec3(cosf(glm::radians(angle-90.f)), sinf(glm::radians(angle-90.f)), 0.f));
        const vec3 fwd = normalize(vec3(cosf(glm::radians(angle)), sinf(glm::radians(angle)), 0.f));
        const vec3 pos = b2Vec2Tovec3(body->GetPosition());

        const b2AABB &body_aabb = body->GetFixtureList()->GetAABB(0);
        const vec3 size = b2Vec2Tovec3(body_aabb.GetExtents());


        dataPtr[0*2] = pos - size.y * ENTITY_SIZE * right - size.x * ENTITY_SIZE * fwd;
        dataPtr[0*2+1] = pos + size.y * ENTITY_SIZE * right - size.x * ENTITY_SIZE * fwd;
        dataPtr[1*2] = pos + size.y * ENTITY_SIZE * right - size.x * ENTITY_SIZE * fwd;
        dataPtr[1*2+1] = pos + size.y * ENTITY_SIZE * right + size.x * ENTITY_SIZE * fwd;
        dataPtr[2*2] = pos + size.y * ENTITY_SIZE * right + size.x * ENTITY_SIZE * fwd;
        dataPtr[2*2+1] = pos - size.y * ENTITY_SIZE * right + size.x * ENTITY_SIZE * fwd;
        dataPtr[3*2] = pos - size.y * ENTITY_SIZE * right + size.x * ENTITY_SIZE * fwd;
        dataPtr[3*2+1] = pos - size.y * ENTITY_SIZE * right - size.x * ENTITY_SIZE * fwd;

        // Display wheels
//        Misc::MakeWheel(&dataPtr[3*2], &bodies[e->bodyIndex+Misc::CAR_WHEEL_FL]);
//        Misc::MakeWheel(&dataPtr[7*2], Misc::GetSubpart(&body, Misc::CAR_WHEEL_FR));
//        Misc::MakeWheel(&dataPtr[11*2], Misc::GetSubpart(&body, Misc::CAR_WHEEL_RL));
//        Misc::MakeWheel(&dataPtr[15*2], Misc::GetSubpart(&body, Misc::CAR_WHEEL_RR));

        // Display debug visuals
//        dataPtr[(Misc::lineCount-2)*2] = pos;
//        dataPtr[(Misc::lineCount-2)*2+1] = pos + ENTITY_SIZE * right;
//        dataPtr[(Misc::lineCount-1)*2] = pos;
//        dataPtr[(Misc::lineCount-1)*2+1] = pos + body.GetForwardVelocity() / 6.f;
    }

    void Helper::FillColors(glm::vec3 *dataPtr, const Entity *e) {
        for(size_t i = 0; i < Misc::lineCount; ++i) {
            dataPtr[2*i] = e->color;
            dataPtr[2*i+1] = e->color;
        }

        // color of debug visuals
//        dataPtr[(Misc::lineCount-2)*2] = vec3(1,0,0);
//        dataPtr[(Misc::lineCount-2)*2+1] = vec3(1,0,0);
//        dataPtr[(Misc::lineCount-1)*2] = vec3(0,1,0);
//        dataPtr[(Misc::lineCount-1)*2+1] = vec3(0,1,0);
    }

    size_t Helper::LineCount() {
        return Misc::lineCount;
    }
/*
    void Misc::MakeWheel(vec3 *dataPtr, const DynamicBody *wheel) {
//        const DynamicBody *wheelBody = GetSubpart(&car, wheelID);
        const vec3 fwd = ENTITY_SIZE * wheel->forwardVector;
        const vec3 right = ENTITY_SIZE * wheel->rightVector;
        const vec3 pos = wheel->position;

        dataPtr[0*2] = pos - wheelWidth * right + wheelHeight * fwd;
        dataPtr[0*2+1] = pos + wheelWidth * right + wheelHeight * fwd;
        dataPtr[1*2] = pos - wheelWidth * right + wheelHeight * fwd;
        dataPtr[1*2+1] = pos - wheelWidth * right - wheelHeight * fwd;
        dataPtr[2*2] = pos - wheelWidth * right - wheelHeight * fwd;
        dataPtr[2*2+1] = pos + wheelWidth * right - wheelHeight * fwd;
        dataPtr[3*2] = pos + wheelWidth * right + wheelHeight * fwd;
        dataPtr[3*2+1] = pos + wheelWidth * right - wheelHeight * fwd;
    }
*/
    // Definition of CarEntity static variables
    const size_t Misc::bodyCount = 5;
    const size_t Misc::lineCount = 4;
    const float Misc::wheelWidth = 0.075f;
    const float Misc::wheelHeight = 0.2f;

    Helper carHelper;

    /*
    void WheelDisplay::fillPositions(glm::vec3 *dataPtr, const Entity &e) {

        const vec3 fwd = ENTITY_SIZE * e->forwardVector;
        const vec3 right = ENTITY_SIZE * e->rightVector;
        const vec3 pos = e->position;

        float w = CarEntity::wheelWidth;
        float h = CarEntity::wheelHeight;

        dataPtr[0*2] = pos - w * right + h * fwd;
        dataPtr[0*2+1] = pos + w * right + h * fwd;
        dataPtr[1*2] = pos - w * right + h * fwd;
        dataPtr[1*2+1] = pos - w * right - h * fwd;
        dataPtr[2*2] = pos - w * right - h * fwd;
        dataPtr[2*2+1] = pos + w * right - h * fwd;
        dataPtr[3*2] = pos + w * right + h * fwd;
        dataPtr[3*2+1] = pos + w * right - h * fwd;
    }
    */

    /*
    void CarEntity::Display_FillPositions(vec3 *dataPtr, const Entity &e) {

        // Create wheels
    //    Display_MakeWheel(&dataPtr[3*2], body, CAR_WHEEL_FL);
    //    Display_MakeWheel(&dataPtr[7*2], body, CAR_WHEEL_FR);
    //    Display_MakeWheel(&dataPtr[11*2], body, CAR_WHEEL_RL);
    //    Display_MakeWheel(&dataPtr[15*2], body, CAR_WHEEL_RR);
        //        MakeWheelAt(&dataPtr[3*2], pos + 1.5f * ENTITY_SIZE * fwd - 0.5f * ENTITY_SIZE * right, ENTITY_SIZE * fwd, ENTITY_SIZE * right);
        //        MakeWheelAt(&dataPtr[7*2], pos + 1.5f * ENTITY_SIZE * fwd + 0.5f * ENTITY_SIZE * right, ENTITY_SIZE * fwd, ENTITY_SIZE * right);
        //        MakeWheelAt(&dataPtr[11*2], pos - 0.6f * ENTITY_SIZE * right, ENTITY_SIZE * fwd, ENTITY_SIZE * right);
        //        MakeWheelAt(&dataPtr[15*2], pos + 0.6f * ENTITY_SIZE * right, ENTITY_SIZE * fwd, ENTITY_SIZE * right);

        // debug visuals
        dataPtr[(lineCount-2)*2] = pos;
        dataPtr[(lineCount-2)*2+1] = pos + ENTITY_SIZE * right;
        dataPtr[(lineCount-1)*2] = pos;
        dataPtr[(lineCount-1)*2+1] = pos + body.GetForwardVelocity() / 6.f;
    }
    */
}


// ###################################################


/*
void DynamicBody::Update(double dt) {
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
    thrust_force = desiredThrustDir * maxThrust * forwardVector * ENTITY_SIZE;;

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

float DynamicBody::GetForwardSpeed() const {
    return dot(forwardVector, velocity);
}

vec3 DynamicBody::GetForwardVelocity() const {
    return forwardVector * dot(forwardVector, velocity);
}

vec3 DynamicBody::GetLateralVelocity() const {
    return rightVector * dot(rightVector, velocity);
}
*/

// ###################################################

Entity::Entity(EntityManager *em, EntityHelper &helper)  : entityManager(em), helperClass(helper)/*: mass(1.f), maxThrust(50.f), maxForwardSpeed(50.f), maxBackwardSpeed(-10.f),
                       angle(0.f), turning(0), desiredSpeed(0.f) */{

}

void Entity::Update(double dt) {
//    body.Update(dt);
}

//const DynamicBody &Entity::GetBody() const { return entityManager->GetBody(bodyIndex); }
//DynamicBody &Entity::GetBody()  { return entityManager->GetBody(bodyIndex); }

/*
void Entity::MakeMesh(vec3 *dataPositionPtr, vec3 *dataColorPtr) {
    vec3 *pos_ptr = dataPositionPtr + dataPositionIndex;
    vec3 *col_ptr = dataColorPtr + dataColorIndex;
    displayFunc.fillPositions(pos_ptr, *this);
    displayFunc.fillColors(col_ptr, *this);
}*/

// #############################################

EntityManager::EntityManager() : world(NULL), array_modification(false) {

}

EntityManager::~EntityManager() {
    if(world) delete world;
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
    filledSize = 0;
    srand(time(NULL));


    makeVBO();

    glEnableVertexArrayAttrib(pPipelineEntities->glidVao, 0);
    glVertexArrayVertexBuffer(pPipelineEntities->glidVao, 0, glidEntitiesPositions, 0, sizeof(vec3));
    glVertexArrayAttribFormat(pPipelineEntities->glidVao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(pPipelineEntities->glidVao, 1);
    glVertexArrayVertexBuffer(pPipelineEntities->glidVao, 1, glidEntitiesColors, 0, sizeof(vec3));
    glVertexArrayAttribFormat(pPipelineEntities->glidVao, 1, 3, GL_FLOAT, GL_FALSE, 0);

    world = new b2World(b2Vec2(0,0));

    EntityDesc e;
    e.position = vec3(-5 * ENTITY_SIZE,0,1);
    e.angle = 0.f;//35.f;
    e.color = vec3(1,0.8,0);
    e.type = Entity::CONTROLLER_PLAYER;
//    e.mass = 1;
    AddEntity(e);

    /*
    for(size_t i = 0; i < 20; ++i) {
        Entity e;
        e.body.position = vec3((rand01() * 40 - 20)  * ENTITY_SIZE, (rand01() * 40 - 20)  * ENTITY_SIZE, 1);
        e.body.angle = rand01() * 360;
        e.color = vec3(rand01() * 0.7 + 0.3, rand01() * 0.7 + 0.3, rand01() * 0.7 + 0.3);
        e.type = Entity::CONTROLLER_AI;
        e.body.mass = 1;
        AddEntity(e);
    }*/

    return true;
}

void EntityManager::AddEntity(EntityDesc &edesc) {
    // Create associated Dynamic Body
    Entity e(this, Car::carHelper);
    e.entityIndex = entities.size();
//    e.bodyIndex = bodies.size();
    e.dataPositionIndex = filledSize;//entityPositions.size();
    e.dataColorIndex = filledSize;//entityColors.size();
    e.color = edesc.color;
    e.type = edesc.type;

    b2BodyDef b_def;
    b_def.type = b2_dynamicBody;
    b_def.position = vec3Tob2Vec2(edesc.position);
    b_def.angle = edesc.angle;
    e.body = world->CreateBody(&b_def);

    b2PolygonShape b_shape;
    b_shape.SetAsBox(1.25f, 0.5f);
    b2FixtureDef b_fixture;
    b_fixture.shape = &b_shape;
    b_fixture.density = 1.f;
    b_fixture.friction = 0.3f;
    e.body->CreateFixture(&b_fixture);
//    e.helperClass.BuildBody(bodies, &e);
//    e.helperClass.SetPosition(edesc.position, &e);
//    e.helperClass.SetAngle(edesc.angle, &e);
    filledSize += 2 * e.helperClass.LineCount();


    // resize arrays/vbos if we get too much entities
    if(filledSize > dataSize) {
        while(filledSize > dataSize)
            dataSize *= 2;
        makeVBO();
    }


    e.helperClass.FillPositions(&entityPositions[e.dataPositionIndex], &e);
    e.helperClass.FillColors(&entityColors[e.dataColorIndex], &e);
//    BasicCarDef::FillPositions(&entityPositions[e.entityIndex*BasicCarDef::LineCount()*2], e);
//    BasicCarDef::FillColors(&entityColors[e.entityIndex*BasicCarDef::LineCount()*2], e);
    entities.push_back(e);


    // Controller type
    if(e.type == Entity::CONTROLLER_PLAYER) {
        std::cout << e.entityIndex << std::endl;
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
    // Update physic
//    for(size_t i = 0; i < bodies.size(); ++i) {
//        DynamicBody &b = bodies[i];
//        b.Update(dt);
//    }

    for(size_t i = 0; i < entities.size(); ++i) {
        Entity &e = entities[i];

        e.Update(dt);


        // TODO : Find when an entity's rendering data shouldnt be updated
        e.helperClass.FillPositions(&entityPositions[e.dataPositionIndex], &e);
//        e.MakeMesh(entityPositions.data(), entityColors.data());
//        BasicCarDef::FillPositions(&entityPositions[e.entityIndex*BasicCarDef::LineCount()*2], e);
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
   /* Entity &e = entityManager->GetEntity(entityIndex);

    if(action == GLFW_PRESS){
        if(key == GLFW_KEY_KP_4) { e.body.turning = 1; }
        if(key == GLFW_KEY_KP_6) { e.body.turning = -1;}

        if(key == GLFW_KEY_KP_8) { e.body.desiredThrustDir = 1;}
        if(key == GLFW_KEY_KP_5) { e.body.desiredThrustDir = -1;}

        if(key == GLFW_KEY_F5) {
            e.body.position = vec3(-5 * ENTITY_SIZE,0,1);
            e.body.acceleration = e.body.velocity = vec3(0,0,0);
            e.body.angle = e.body.desiredThrustDir= 0.f;
            e.body.turning = 0;
        }
    }

    if(action == GLFW_RELEASE) {
        if((key == GLFW_KEY_KP_4 && e.body.turning == 1) ||
                (key == GLFW_KEY_KP_6 && e.body.turning == -1)) {
            e.body.turning = 0;
        }
        if((key == GLFW_KEY_KP_8 && e.body.desiredThrustDir > 0.f) ||
                (key == GLFW_KEY_KP_5 && e.body.desiredThrustDir < 0.f)) {
            e.body.desiredThrustDir = 0.f;
        }
    }*/
}

void AIController::Update() {
}
