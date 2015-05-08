#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

#include "glm/glm.hpp"
#include "program_pipeline.h"

class EntityManager;
class EntityType;



// ####################################


/// Get a Unit direction vector from an angle in radians
inline glm::vec3 AngleToVector(float angle) {
    return glm::vec3(std::cos(angle), std::sin(angle), 0.f);
}

/// Get a scalar angle value in radians from a direction vector
inline float VectorToAngle(const glm::vec3 &v) {
    return (float)std::atan2(v.y, v.x);
}

struct Body {
friend class EntityManager;
friend class PlayerController;
friend class AIController;
    Body();

    void Update(double dt, int control_state);

    /// Regenerate the body local space from the angle
    void GenerateLocalSpace();

    /// Reset the body local space to
    void ResetLocalSpace();
    glm::vec3 ToLocal(glm::vec3 global_vector) const;
    glm::vec3 ToGlobal(glm::vec3 local_vector) const;

    float GetForwardSpeed() const;
    glm::vec3 GetForwardVelocity() const;
    glm::vec3 GetLateralVelocity() const;

    glm::vec3 position;        //!< Body position in space
    glm::vec3 forward;        //!< Forward body vector
    glm::vec3 right;            //!< Right body vector, orthogonal to forward

    glm::vec3 velocity;
    glm::vec3 acceleration;

    float mass;
    float angle;

    // TODO : differentiate bodies between types ?? (pedestrian, 4wheeled, plane, etc)
    int speedMult;      //!< In a car, the current gear level. for character, the run level, etc..
    int rpm;
    float wheel_velocity;
    float wheel_acceleration;

    bool awake;

private:
    size_t bodyIndex;
    EntityType *type;
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

    enum ControlState {
        CS_FWD = 1 << 1,
        CS_BWD = 1 << 2,
        CS_LEFT = 1 << 3,
        CS_RIGHT = 1 << 4,
        CS_NONE = 1 << 5
    };

public:
    Entity();

    // @return : true if the entity has been updated
    void Update(double dt);

//    Body &GetBody() { return body; }
//    const Body &GetBody() const { return body; }


public:
    glm::vec3 color;
    ControllerType controller;
    int controlState;

private:
    size_t entityIndex;
    size_t dataIndex;
    EntityType *type;
};

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

/// Helper class describing a type of entity in particular (e.g. pedestrian, car)
/// Inherit from it to create a new type of entity
class EntityType {
public:
    virtual void MakeDataPositions(glm::vec3 *target, const Body &b) { assert(0); }
    virtual void MakeDataColors(glm::vec3 *target, const Entity &e) { assert(0); }

    /// Update the logic for the given body, according to the derived Entity Type
    virtual void UpdateLogic(float dt, Body &b, int control_state) { assert(0); }

    size_t segmentCount;         //!< Number of segments for drawing the entity

    // Physic
    float mass;                       //!< Body mass
    float maxThrust;                //!< Engine force output, altered by gear level to compute speed

    // Car specific
    int forwardGears;               //!< Number of gears for forward propulsion
    int backwardGears;            //!< Number of gears for backward propulsion
    float gearRatios[7];            //!< Values of gear ratios for each gear. TMP : max=7 gears
    float differentialRatio;
    float wheelRadius;
    float resistanceDrag;
    float resistanceRolling;
};

// ####################################

struct EntityDesc {
    EntityDesc();
    glm::vec3 position;
    float angle;
    glm::vec3 color;
    EntityType *type;
    Entity::ControllerType controller;
};


class EntityManager {
public:
    EntityManager();

    bool Init();
    void KeyCallback(int key, int action);
    void Update(double dt);
    void Render(const glm::mat4 &viewMatrix);

    void AddEntity(const EntityDesc &entity_desc);

    Body &GetBody(size_t index) { assert(index < bodies.size()); return bodies[index]; }
    Entity &GetEntity(size_t index) { assert(index < entities.size()); return entities[index]; }
    size_t GetEntityCount() const { return entities.size(); }

private:
    /// Entities & Bodies have a 1:1 relationship. Body at index i is used by entity at index i.
    std::vector<Entity> entities;
    std::vector<Body> bodies;

    PlayerController playerController;
    std::vector<AIController> aiControllers;

    // internal rendering stuff
    ProgramPipeline* pPipelineEntities;
    bool array_modification;

    size_t dataSize;
    size_t filledSize;

    std::vector<glm::vec3> entityPositions;
    GLuint glidEntitiesPositions;

    std::vector<glm::vec3> entityColors;
    GLuint glidEntitiesColors;

    void makeVBO();
};


#endif // ENTITY_H
