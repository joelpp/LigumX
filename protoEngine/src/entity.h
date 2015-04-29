#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

#include "glm/glm.hpp"
#include "program_pipeline.h"


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


    float GetForwardSpeed() const;
    glm::vec3 GetForwardVelocity() const;
    glm::vec3 GetLateralVelocity() const;

public:
    // Entity features
    float mass;                        //!< Entity Mass
    float maxThrust;                 //!< Maximum thrust acceleration
    float maxForwardSpeed;      //!< Maximum atteignable forward speed
    float maxBackwardSpeed;    //!< Maximum atteignable backward speed

    glm::vec3 color;
    ControllerType type;


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
