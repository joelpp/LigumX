#version 410 core
layout(location=0) in vec3 pos;
layout(location=1) in vec2 texCoord;

uniform mat4 vpMat;
uniform mat4 modelMatrix;
uniform vec2 offset;
uniform sampler2D heightmap;
out vec2 myTexCoord;
out float height;
out gl_PerVertex {
    vec4 gl_Position;
};


void main() {
    float sampledHeight = texture(heightmap, vec2(pos.x, pos.y)).r;
    vec3 vertexPosition = vec3(pos.x + offset.x, pos.y + offset.y, sampledHeight);
    gl_Position = vpMat * modelMatrix * vec4(vertexPosition, 1);
    myTexCoord = texCoord;
    height = sampledHeight;
}
