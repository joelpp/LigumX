#version 410 core
layout (location = 0) in vec3 vertex; // <vec2 pos, vec2 tex>
layout (location = 1) in vec2 uvs;
out vec2 TexCoords;

uniform mat4 projection;
out gl_PerVertex {
    vec4 gl_Position;
};
void main()
{
    gl_Position = projection * vec4(vertex.xyz, 1.0);
    TexCoords = uvs;
}
