#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <Box2D/Box2D.h>

#include "glm/glm.hpp"
#include "program_pipeline.h"

class EntityManager;


inline glm::vec3 b2Vec2Tovec3( const b2Vec2 &v ) {
    return glm::vec3( v.x, v.y, 1.f );
}

inline b2Vec2 vec3Tob2Vec2( const glm::vec3 &v ) {
    return b2Vec2( v.x, v.y );
}

inline glm::vec3 getForwardVelocity(const b2Body *body) {
      b2Vec2 currentRightNormal = body->GetWorldVector( b2Vec2(1,0) );
      return b2Vec2Tovec3(b2Dot( currentRightNormal, body->GetLinearVelocity() ) * currentRightNormal);
}

inline glm::vec3 getLateralVelocity(const b2Body *body) {
      b2Vec2 currentRightNormal = body->GetWorldVector( b2Vec2(1,0) );
      return b2Vec2Tovec3(b2Dot( currentRightNormal, body->GetLinearVelocity() ) * currentRightNormal);
}

/*
class DynamicBody {
public:
public:
    DynamicBody() : mass(1.f), maxThrust(50.f), angle(0.f), turning(0), desiredThrustDir(0) {}

    void Update(double dt);
//    void RebuildMesh(glm::vec3 *dataPtr);

    float GetForwardSpeed() const;
    glm::vec3 GetForwardVelocity() const;
    glm::vec3 GetLateralVelocity() const;

public:
    float mass;                        //!< Entity Mass
    float maxThrust;                 //!< Maximum thrust acceleration

    float angle;
    int turning;
    int desiredThrustDir;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 forwardVector;
    glm::vec3 rightVector;


private:
//    float maxForwardSpeed;      //!< Maximum atteignable forward speed
//    float maxBackwardSpeed;    //!< Maximum atteignable backward speed
};
*/
class Entity;

class EntityHelper {
public:
    // display
    virtual void FillPositions(glm::vec3 *dataPtr, const Entity *e) {assert(0);}
    virtual void FillColors(glm::vec3 *dataPtr, const Entity *e) {assert(0);}
    virtual size_t LineCount() {assert(0);}

    // physics
//    virtual void BuildBody(std::vector<DynamicBody> &dataPtr, const Entity *e) {assert(0);}
    virtual void SetPosition(const glm::vec3 &pos, Entity *e) {assert(0);}
    virtual void SetAngle(float angle, Entity *e) {assert(0);}
};


class Entity {
friend class EntityManager;
friend class PlayerController;
friend class AIController;
public :
    enum ControllerType {
        CONTROLLER_PLAYER,
        CONTROLLER_AI
    };

public:
    Entity(EntityManager *em, EntityHelper &helper);

    // @return : true if the entity has been updated
    void Update(double dt);

//    void MakeMesh(glm::vec3 *dataPositionPtr, glm::vec3 *dataColorPtr);

    const b2Body *GetBody() const { return body; }
    b2Body *GetBody() { return body; }

public:
    // Entity features
    glm::vec3 color;
    ControllerType type;


    size_t entityIndex;
    size_t dataPositionIndex, dataColorIndex;
    size_t bodyIndex;
private:
    EntityManager *entityManager;

    EntityHelper &helperClass;

    // TODO : array of DynamicBodies in a PhysicManager for cache coherence
    b2Body *body;
//    DynamicBody body;
};




namespace Car {
    class Misc {
    public:
        enum CarPart {
            CAR_HULL,
            CAR_WHEEL_FL,
            CAR_WHEEL_FR,
            CAR_WHEEL_RL,
            CAR_WHEEL_RR,
        };

//        static void MakeWheel(glm::vec3 *dataPtr, const DynamicBody *wheel);

//        static const DynamicBody *GetSubpart(const DynamicBody *bodies, CarPart part) {
//            return (DynamicBody*)(bodies+(int)part * sizeof(DynamicBody));
//        }

//        static DynamicBody *GetSubpart(DynamicBody *bodies, CarPart part) {
//            return (DynamicBody*)(bodies+(int)part * sizeof(DynamicBody));
//        }

        static const size_t bodyCount;
        static const size_t lineCount;
        static const float wheelWidth;
        static const float wheelHeight;

    };


    class Helper : public EntityHelper {
    public:
        virtual void FillPositions(glm::vec3 *dataPtr, const Entity *e);
        virtual void FillColors(glm::vec3 *dataPtr, const Entity *e);
        virtual size_t LineCount();
//        virtual void BuildBody(std::vector<DynamicBody> &dataPtr, const Entity *e);

        virtual void SetPosition(const glm::vec3 &pos, Entity *e);
        virtual void SetAngle(float angle, Entity *e);
    };
}

// ####################################

class EntityController {
public:
    EntityController(EntityManager *e, int ei) : entityManager(e), entityIndex(ei) {}
    virtual ~EntityController() {}

    virtual void Update() {}
    virtual void OnKey(int key, int action) {}

protected:
    EntityManager *entityManager;
    int entityIndex;                     //!< Index of controlled entity in manager array
};

class PlayerController : public EntityController {
public:
    PlayerController(EntityManager *e = NULL, int entityIndex = -1) : EntityController(e, entityIndex) {}

    void Update();
    void OnKey(int key, int action);
};

class AIController : public EntityController {
public:
    AIController(EntityManager *e, int entityIndex = -1) : EntityController(e, entityIndex) {}

    void Update();
};

// ####################################

struct EntityDesc {
    glm::vec3 position;
    float angle;
    glm::vec3 color;
//    float mass;
    Entity::ControllerType type;
};

class EntityManager {
public:
    EntityManager();
    ~EntityManager();

    bool Init();
    void KeyCallback(int key, int action);
    void Update(double dt);
    void Render(const glm::mat4 &viewMatrix);

    void AddEntity(EntityDesc &edesc);

    Entity &GetEntity(size_t index) { assert(index < entities.size()); return entities[index]; }
    const Entity &GetEntity(size_t index) const { assert(index < entities.size()); return entities[index]; }
    size_t GetEntityCount() const { return entities.size(); }

//    DynamicBody &GetBody(size_t index) { assert(index < bodies.size()); return bodies[index]; }
//    const DynamicBody &GetBody(size_t index) const { assert(index < bodies.size()); return bodies[index]; }
private:
    b2World *world;

    std::vector<Entity> entities;
//    std::vector<DynamicBody> bodies;

    PlayerController playerController;
    std::vector<AIController> aiControllers;

    // internal rendering stuff
    ProgramPipeline* pPipelineEntities;
    bool array_modification;

    std::vector<glm::vec3> entityPositions;
    GLuint glidEntitiesPositions;

    std::vector<glm::vec3> entityColors;
    GLuint glidEntitiesColors;

    size_t dataSize;
    size_t filledSize;

    void makeVBO();
};


#endif // ENTITY_H
