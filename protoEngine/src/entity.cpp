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

    Helper::Helper() {
        size = vec3(1.25f, 0.5f, 0.f);
        mass = 1.f;
        maxThrust = 100.f;
        maxTorque = 0.1f;
        maxLateralImpulse = 3.f;
        resistanceDrag = 0.4257f;
        resistanceRolling = 30.f * resistanceDrag;
    }

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
        const vec3 right = b2Vec2Tovec3(body->GetWorldVector(b2Vec2(0,1)));
        const b2Vec2 b2fwd = body->GetWorldVector(b2Vec2(1,0));
        const vec3 fwd = b2Vec2Tovec3(b2fwd);
        const vec3 pos = ENTITY_SIZE * b2Vec2Tovec3(body->GetPosition());

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
        dataPtr[(Misc::lineCount-1)*2] = pos;
        dataPtr[(Misc::lineCount-1)*2+1] = pos + ENTITY_SIZE * fwd * b2Dot(body->GetLinearVelocity(),b2fwd) /5.f;
//        dataPtr[(Misc::lineCount-1)*2] = pos;
//        dataPtr[(Misc::lineCount-1)*2+1] = pos + body.GetForwardVelocity() / 6.f;
    }

    void Helper::FillColors(glm::vec3 *dataPtr, const Entity *e) {
        for(size_t i = 0; i < Misc::lineCount-1; ++i) {
            dataPtr[2*i] = e->color;
            dataPtr[2*i+1] = e->color;
        }

        // color of debug visuals
//        dataPtr[(Misc::lineCount-2)*2] = vec3(1,0,0);
//        dataPtr[(Misc::lineCount-2)*2+1] = vec3(1,0,0);
        dataPtr[(Misc::lineCount-1)*2] = vec3(0,1,0);
        dataPtr[(Misc::lineCount-1)*2+1] = vec3(0,1,0);
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
    const size_t Misc::lineCount = 5;
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

Entity::Entity(EntityManager *em, EntityHelper &helper)  : entityManager(em), helperClass(helper), controlState(0){

}

void Entity::Update(double dt) {
    float engine_force = 0.f;
    float air_resistance = 0.f;
    float rolling_resistance = 0.f;


    // F_traction = fwd_normal * engineForce
    if(controlState & CS_FWD)  engine_force = helperClass.maxThrust;
    else if(controlState & CS_BWD) engine_force = -helperClass.maxThrust/5.f;



    b2Vec2 fwd_normal = body->GetWorldVector(b2Vec2(1,0));
    b2Vec2 velocity_vec = vec3Tob2Vec2(getForwardVelocity(body));
    float speed = velocity_vec.Length();

    if(speed) {
        // F_drag = - C_drag * vel * |vel|
        air_resistance = -helperClass.resistanceDrag * speed;
        rolling_resistance = -helperClass.resistanceRolling;
    }

    b2Vec2 drive_force = engine_force * fwd_normal + (air_resistance + rolling_resistance) * velocity_vec;

    body->ApplyForce(drive_force, body->GetWorldCenter(), 1);

    float torque = 0.f;
    if(controlState & CS_LEFT) { torque = helperClass.maxTorque; }
    if(controlState & CS_RIGHT) {torque = -helperClass.maxTorque; }
    body->ApplyAngularImpulse(torque,1);

    // allow skidding
    b2Vec2 lateral_correction = body->GetMass() * -vec3Tob2Vec2(getLateralVelocity(body));
    float impulse_strength = lateral_correction.Length();
    if (impulse_strength > helperClass.maxLateralImpulse)
        lateral_correction *= helperClass.maxLateralImpulse / impulse_strength;
    body->ApplyLinearImpulse(lateral_correction, body->GetWorldCenter(), 1);

    // remove angular velocity while allowing skidding
    body->ApplyAngularImpulse( 0.1f * body->GetInertia() * -body->GetAngularVelocity(), 1 );
}

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
    e.position = vec3(-5, 0,1);
    e.angle = 0.f;//35.f;
    e.color = vec3(1,0.8,0);
    e.type = Entity::CONTROLLER_PLAYER;
//    e.mass = 1;
    AddEntity(e);

    e.position = vec3(5, 0,1);
    e.angle = 0.f;//35.f;
    e.color = vec3(1,1,0);
    e.type = Entity::CONTROLLER_AI;
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
    Entity e(this, Car::carHelper);
    e.entityIndex = entities.size();
    e.dataPositionIndex = filledSize;
    e.dataColorIndex = filledSize;
    e.color = edesc.color;
    e.type = edesc.type;

    // Create associated Dynamic Body
    b2BodyDef b_def;
    b_def.type = b2_dynamicBody;
    b_def.position = vec3Tob2Vec2(edesc.position);
    b_def.angle = edesc.angle;
    e.body = world->CreateBody(&b_def);

    b2PolygonShape b_shape;
    b_shape.SetAsBox(Car::carHelper.size.x, Car::carHelper.size.y);
    b2FixtureDef b_fixture;
    b_fixture.shape = &b_shape;
    b_fixture.density = 1.f;
    b_fixture.friction = 0.3f;
    e.body->CreateFixture(&b_fixture);
    filledSize += 2 * e.helperClass.LineCount();    // 2 vertices per line


    // resize arrays/vbos if we get too much entities
    if(filledSize > dataSize) {
        while(filledSize > dataSize)
            dataSize *= 2;
        makeVBO();
    }


    e.helperClass.FillPositions(&entityPositions[e.dataPositionIndex], &e);
    e.helperClass.FillColors(&entityColors[e.dataColorIndex], &e);
    entities.push_back(e);


    // Controller type
    if(e.type == Entity::CONTROLLER_PLAYER) {
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

        e.Update(dt);

        // TODO : Find when an entity's rendering data shouldnt be updated
        e.helperClass.FillPositions(&entityPositions[e.dataPositionIndex], &e);
        array_modification = true;
    }

    world->Step((float)dt, 8, 3);
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

    if(action == GLFW_PRESS){
        if(key == GLFW_KEY_KP_4) { e.controlState |= Entity::CS_LEFT; }
        if(key == GLFW_KEY_KP_6) { e.controlState |= Entity::CS_RIGHT;}

        if(key == GLFW_KEY_KP_8) { e.controlState |= Entity::CS_FWD; }
        if(key == GLFW_KEY_KP_5) { e.controlState |= Entity::CS_BWD; }

        if(key == GLFW_KEY_F5) {
//            e.body.position = vec3(-5 * ENTITY_SIZE,0,1);
//            e.body.acceleration = e.body.velocity = vec3(0,0,0);
//            e.body.angle = e.body.desiredThrustDir= 0.f;
//            e.body.turning = 0;
        }
    }

    if(action == GLFW_RELEASE) {
        if(key == GLFW_KEY_KP_4) { e.controlState &= ~Entity::CS_LEFT; }
        if(key == GLFW_KEY_KP_6) { e.controlState &= ~Entity::CS_RIGHT;}

        if(key == GLFW_KEY_KP_8) { e.controlState &= ~Entity::CS_FWD; }
        if(key == GLFW_KEY_KP_5) { e.controlState &= ~Entity::CS_BWD; }
    }
}

void AIController::Update() {
}
