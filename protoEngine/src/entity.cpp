#include "entity.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

using namespace glm;

Entity::Entity() : angle(0.f), fwd_force(0.f), turning(0) {

}

bool Entity::Update(double dt) {
    if(turning) {
        angle += dt * 1.f * turning;
        vec3 curr_angle(cosf(glm::radians(angle)), sinf(glm::radians(angle)), 0);
        std::cout << angle << std::endl;

        acceleration = curr_angle * fwd_force;
//        std::cout << to_string(acceleration) << std::endl;
        velocity = acceleration * fwd_force;
        position += velocity / (float)dt;
//        std::cout << to_string(position) << std::endl;
        return true;
    }
    return false;
}

// #############################################

EntityManager::EntityManager() : array_modification(false) {

}

bool EntityManager::Init() {
    const char* vertexShaderSource = " \
        #version 430 core\n \
        in layout(location=0) vec2 pos;\n \
        in layout(location=1) vec3 color;\n \
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
        #version 430 core\n \
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
    data_size = 100;


    makeVBO();

    glEnableVertexArrayAttrib(pPipelineEntities->glidVao, 0);
    glVertexArrayVertexBuffer(pPipelineEntities->glidVao, 0, glidEntitiesPositions, 0, sizeof(vec3));
    glVertexArrayAttribFormat(pPipelineEntities->glidVao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(pPipelineEntities->glidVao, 1);
    glVertexArrayVertexBuffer(pPipelineEntities->glidVao, 1, glidEntitiesColors, 0, sizeof(vec3));
    glVertexArrayAttribFormat(pPipelineEntities->glidVao, 1, 3, GL_FLOAT, GL_FALSE, 0);

    Entity e;
    e.position = vec3(0,0,1);
    e.color = vec3(1,0.8,0);
    AddEntity(e);

    return true;
}

void EntityManager::AddEntity(const Entity &entity) {
    Entity e(entity);

    vec3 angle(cosf(glm::radians(e.angle)), sinf(glm::radians(e.angle)), 0.f);

    e.entityIndex = entities.size();

    // resize arrays/vbos if we get too much entities
    if(entities.size() > data_size) {
        data_size *= 2;
        makeVBO();
    }


    entity_positions[e.entityIndex*2] = e.position;
    entity_positions[e.entityIndex*2+1] = e.position + ENTITY_SIZE * angle;
    entity_colors[e.entityIndex*2] = e.color;
    entity_colors[e.entityIndex*2+1] = e.color;
    array_modification = true;

    entities.push_back(e);
}

void EntityManager::makeVBO() {
    entity_positions.resize(data_size * 2);
    entity_colors.resize(data_size * 2);

    glDeleteBuffers(1, &glidEntitiesPositions);
    glCreateBuffers(1, &glidEntitiesPositions);
    glNamedBufferStorage(glidEntitiesPositions, entity_positions.size() * sizeof(vec3),
                         NULL,
                         GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

    glDeleteBuffers(1, &glidEntitiesColors);
    glCreateBuffers(1, &glidEntitiesColors);
    glNamedBufferStorage(glidEntitiesColors, entity_positions.size() * sizeof(vec3),
                         NULL,
                         GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

    array_modification = true;
}

void EntityManager::KeyCallback(int key, int action) {
    Entity &e = entities[0];

    if(action == GLFW_PRESS){
        if(key == GLFW_KEY_KP_4) { e.turning = 1; }
        if(key == GLFW_KEY_KP_6) { e.turning = -1;}

        if(key == GLFW_KEY_KP_8) { e.fwd_force = 1;}
        if(key == GLFW_KEY_KP_5) { e.fwd_force = -1;}
    }

    if(action == GLFW_RELEASE) {
        if((key == GLFW_KEY_KP_4 && e.turning == 1) || (key == GLFW_KEY_KP_6 && e.turning == -1)) {
            e.turning = 0;
        }
        if((key == GLFW_KEY_KP_8 && e.fwd_force == 1) || (key == GLFW_KEY_KP_5 && e.fwd_force == -1)) {
            e.fwd_force = 0;
        }
    }
}

void EntityManager::Update(double dt) {
    for(size_t i = 0; i < entities.size(); ++i) {
        Entity &e = entities[i];

        if(e.Update(dt)) {
            vec3 entity_dir(cosf(glm::radians(e.angle)), sinf(glm::radians(e.angle)), 0);

            entity_positions[e.entityIndex*2] = e.position;
            entity_positions[e.entityIndex*2+1] = e.position + ENTITY_SIZE * entity_dir;
            array_modification = true;
        }
    }
}

void EntityManager::Render(const mat4 &viewMatrix) {
    if(array_modification) {
        glNamedBufferSubData(glidEntitiesPositions, 0, (entities.size() * 2) * sizeof(vec3), entity_positions.data());
        glNamedBufferSubData(glidEntitiesColors, 0, (entities.size() * 2) * sizeof(vec3), entity_colors.data());
    }

    pPipelineEntities->usePipeline();
    glProgramUniformMatrix4fv(pPipelineEntities->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipelineEntities->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(viewMatrix));
    glDrawArrays(GL_LINES, 0, entities.size() * 2);
}
