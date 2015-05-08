#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

#include "glm/glm.hpp"
#include "program_pipeline.h"

class EntityManager;

struct Body {
    Body();

    void Update(double dt);

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
    int turning;
    float desiredSpeed;

    bool awake;
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
    Entity();

    // @return : true if the entity has been updated
    void Update(double dt);

    Body &GetBody() { return body; }
    const Body &GetBody() const { return body; }


public:
    // Entity features
    float maxThrust;                 //!< Maximum thrust acceleration
    float maxForwardSpeed;      //!< Maximum atteignable forward speed
    float maxBackwardSpeed;    //!< Maximum atteignable backward speed

    glm::vec3 color;
    ControllerType type;


private:
    size_t entityIndex;

    Body body;

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

class EntityManager {
public:
    EntityManager();

    bool Init();
    void KeyCallback(int key, int action);
    void Update(double dt);
    void Render(const glm::mat4 &viewMatrix);

    void AddEntity(const Entity &e);

    Entity &GetEntity(size_t index) { assert(index < entities.size()); return entities[index]; }
    size_t GetEntityCount() const { return entities.size(); }

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
