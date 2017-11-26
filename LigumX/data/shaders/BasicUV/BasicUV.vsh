#version 410 core

layout(location=0) in vec3 pos;
layout(location=1) in vec2 texCoord;

uniform mat4 vpMat;
uniform mat4 modelMatrix;
uniform vec2 offset;
uniform sampler2D heightmap;
//uniform sampler2D g_DiffuseTexture;

out vec2 myTexCoord;
out float height;
out gl_PerVertex 
{
    vec4 gl_Position;
	float gl_PointSize;
	float gl_ClipDistance[];
};


void main() 
{
    float sampledHeight = texture(heightmap, vec2(pos.x, pos.y)).r / 5;
    vec3 vertexPosition = vec3(pos.x * 10.f + 10.f + 10.f * offset.x , pos.y * 10.f + 10.f + 10.f * offset.y, 5.f  * sampledHeight);
    gl_Position = vpMat * modelMatrix * vec4(vertexPosition, 1);
    myTexCoord = texCoord;
    height = sampledHeight;
}