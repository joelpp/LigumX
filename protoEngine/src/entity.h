#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

#include "glm/glm.hpp"
#include "program_pipeline.h"


class Entity {
public:
    Entity();

    // @return : true if the entity has been updated
    bool Update(double dt);

    u_int32_t entityIndex;

    float mass;
    float angle, fwd_force;
    int turning;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration; //float accel_power;

    glm::vec3 color;
};

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

    // internal rendering stuff
    ProgramPipeline* pPipelineEntities;
    bool array_modification;
    size_t data_size;

    std::vector<glm::vec3> entity_positions;
    GLuint glidEntitiesPositions;

    std::vector<glm::vec3> entity_colors;
    GLuint glidEntitiesColors;

    void makeVBO();
};

#endif // ENTITY_H
